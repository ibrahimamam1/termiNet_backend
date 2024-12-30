#define CROW_MAIN

#include "crow_all.h"
#include "db/manager/connection_manager.h"
#include "helpers/ResponseHelper.h"
#include "services/login/login.h"
#include "services/routing/routing.h"
#include "services/users/users.h"
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
    ;

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
      .methods("POST"_method)([](const crow::request &req) {
        crow::json::wvalue response;
        try {
          std::cout << "Received body: " << req.body << std::endl;
          auto json_data = crow::json::load(req.body);
          if (!json_data) {
            response = ResponseHelper::make_response(400, "Invalid Json");
            return response;
          }
          std::cout << "Parsed JSON: " << json_data << std::endl;
          response = UserService::createUser(json_data);
        } catch (std::runtime_error e) {
          std::cerr << e.what();
          response = ResponseHelper::make_response(500, e.what());
        }
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
        } else {
          int dest = message["destination"].i();
          int src = message["source"].i();
          std::string msg = message["content"].s();
          RoutingService::routeMessage(src, dest, msg);
        }
      });

  app.port(8080).multithreaded().run();
}
