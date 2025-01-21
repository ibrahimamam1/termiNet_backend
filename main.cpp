#define CROW_MAIN

#include "crow_all.h"
#include "db/manager/connection_manager.h"
#include "helpers/ResponseHelper.h"
#include "services/login/login.h"
#include "services/routing/routing.h"
#include "services/threads/threads.h"
#include "services/users/users.h"
#include "services/category/category.h"
#include "services/community/community.h"
#include <csignal>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

using webSocketPtr = std::shared_ptr<crow::websocket::connection>;
std::unordered_map<int, webSocketPtr> user_connections;

int main() {
  crow::SimpleApp app;

  // initialise database connection
  ConnectionManager::getInstance()->init("dbname=terminet user=rgb password=");

  CROW_ROUTE(app, "/")([]() { return "Hello World"; });

  // handle login check
  CROW_ROUTE(app, "/login/<string>/<string>")
  ([](std::string email, std::string password) {
    crow::json::wvalue response = LoginService::check_login(email, password);
    return response;
  });

  // handle getting a user information
  CROW_ROUTE(app, "/users/<int>/<string>")
  ([](int code, std::string id) {
    crow::json::wvalue response;

    if (code == 1)
      response = UserService::getUserWithEmail(id);
    else if (code == 2)
      response = UserService::getUserWithName(id);
    else if (code == 3)
      response = UserService::getUserWithUserId(id);

    return response;
  });

  // handle adding new user
  CROW_ROUTE(app, "/users/")
      .methods(crow::HTTPMethod::POST)([](const crow::request &req) {
        crow::json::wvalue response;
        try {
          std::cout << "Creating a new user:\n";
          auto json_data = crow::json::load(req.body);
          if (!json_data) {
            std::cerr << "\tReceived Invalid Json\n";
            response = ResponseHelper::make_response(400, "Invalid Json");
            return response;
          }
          std::cout << "\tjson parsed succesffully\n";
          response = UserService::createUser(json_data);
        } catch (std::runtime_error e) {
          std::cerr << e.what();
          response = ResponseHelper::make_response(500, e.what());
        }
        return response;
      });

  // updating user information
  CROW_ROUTE(app, "/update/users/")
      .methods(crow::HTTPMethod::POST)([](const crow::request &req) {
        crow::json::wvalue response;
        std::string key = req.get_header_value("id");
        try {
          auto json_data = crow::json::load(req.body);
          if (!json_data) {
            response = ResponseHelper::make_response(400, "Invalid Json");
            return response;
          }
          response = UserService::updateUser(json_data["field"].s(),
                                             json_data["new_data"].s(), key);
          return response;
        } catch (std::runtime_error e) {
          std::cerr << e.what();
          response = ResponseHelper::make_response(500, e.what());
          return response;
        }
      });

  // -----------------------------------------Threads-------------------------------------
  
  //adding new threads
  CROW_ROUTE(app, "/threads/new/")
      .methods(crow::HTTPMethod::POST)([](const crow::request &req) {
        crow::json::wvalue response;
        try {
          std::cout << "Creating a new Thread:\n";
          auto json_data = crow::json::load(req.body);
          if (!json_data) {
            std::cerr << "\tReceived Invalid Json\n";
            response = ResponseHelper::make_response(400, "Invalid Json");
            return response;
          }
          std::cout << "\tjson parsed succesffully\n";
          response = ThreadService::addNewThread(json_data);
        } catch (std::runtime_error e) {
          std::cerr << e.what();
          response = ResponseHelper::make_response(500, e.what());
        }
        return response;
      });
  
  //getting threads
  CROW_ROUTE(app, "/threads/<string>/<string>")
  ([](std::string filter, std::string value) {
    crow::json::wvalue response = ThreadService::getThreads(filter,value);
    return response;
  });

  //------------------------------CATEGORY----------------------------------------------  

  //getting categories
  CROW_ROUTE(app, "/categories/<string>/<string>")
  ([](std::string filter, std::string value){
    return CategoryService::getCategories(filter, value);
  });

  //----------------------------COMMUNITITES--------------------------------
  //adding new communities
  CROW_ROUTE(app, "/communities/new/")
      .methods(crow::HTTPMethod::POST)([](const crow::request &req) {
        crow::json::wvalue response;
        try {
          std::cout << "Creating a new Community:\n";
          auto json_data = crow::json::load(req.body);
          if (!json_data) {
            std::cerr << "\tReceived Invalid Json\n";
            response = ResponseHelper::make_response(400, "Invalid Json");
            return response;
          }
          std::cout << "\tjson parsed succesffully\n";
          response = CommunityService::addNewCommunity(json_data);
        } catch (std::runtime_error e) {
          std::cerr << e.what();
          response = ResponseHelper::make_response(500, e.what());
        }
        return response;
      });
  //retrieving communities
  CROW_ROUTE(app, "/communities/<string>/<string>")
  ([](std::string filter, std::string value){
  std::cout << "Community Route: got Your request\n";
      crow::json::wvalue response = CommunityService::getCommunities(filter, value);
      return response;
    });
  //remove user from community
  CROW_ROUTE(app, "/communities/users/remove/<string>/<int>")
  ([](std::string user_id, int community_id){
      crow::json::wvalue response = CommunityService::removeUserFromCommunity(user_id, community_id);
      return response;
    });



  // messaging web socket
  CROW_WEBSOCKET_ROUTE(app, "/ws/")
      .onopen([&](crow::websocket::connection &conn) {
        std::cout << "Received Web Socket Connection Request " << std::endl;
      })
      .onclose([&](crow::websocket::connection &conn, std::string reason) {
        std::cout << reason << std::endl;
        RoutingService::removeConnection(&conn);
      })
      .onmessage([&](crow::websocket::connection &conn, const std::string &data,
                     bool is_binary) {
        crow::json::rvalue message = crow::json::load(data);
        int type = message["type"].i();
        if (type == 1) {
          int key = message["key"].i();
          auto conn_ptr = std::shared_ptr<crow::websocket::connection>(
              &conn, [](auto *) {});
          RoutingService::addConnection(key, conn_ptr);
          RoutingService::sendUndeliveredMessages(key);
        } else {
          int dest = message["destination"].i();
          int src = message["source"].i();
          std::string msg = message["content"].s();
          RoutingService::routeMessage(src, dest, msg);
        }
      });

  app.port(8080).multithreaded().run();
}
