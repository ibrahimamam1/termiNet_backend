#include <crow.h>
#include <crow/app.h>
#include <crow/json.h>
#include <crow/websocket.h>
#include <csignal>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "services/login/login.h"
#include "services/users/users.h"
#include "helpers/ResponseHelper.h"
#include "db/manager/connection_manager.h"

#define CROW_MAIN

using webSocketPtr = std::shared_ptr<crow::websocket::connection>;
std::unordered_map<int, webSocketPtr> user_connections;

int main(){
  crow::SimpleApp app;

  //signal handlers for graceful shutdown
  std::signal(SIGINT, [](int signal){
    std::cout << "Shuting Down Server\n";
    ConnectionManager::getInstance()->closeConnections(); 
  });

  std::signal(SIGTERM, [](int signal){
    std::cout << "Shuting Down Server\n";
    ConnectionManager::getInstance()->closeConnections(); 
  });

  //initialise database connection
  ConnectionManager::getInstance()->init("dbname=terminet user=rgb password=");

  CROW_ROUTE(app, "/")([](){
    return "Hello World";
  });
  
  // handle login check
  CROW_ROUTE(app, "/login/<string>/<string>")([](std::string email, std::string password){ 
    crow::json::wvalue response = LoginService::check_login(email, password);
    return response;
  });

  //handle getting a user information
  CROW_ROUTE(app, "/users/<int>/<string>")([](int code, std::string id){
    crow::json::wvalue response;;
    
    if(code == 1)
      response = UserService::getUserWithEmail(id);
    else if(code == 2)
      response = UserService::getUserWithName(id);
    else if(code == 3)
      response = UserService::getUserWithUserId(id);
    
   return response;
  });
  
  //handle adding new user
  CROW_ROUTE(app, "/users/").methods("POST"_method)([](const crow::request& req){
    crow::json::wvalue response;
    try{
      std::cout << "Received body: " << req.body << std::endl;
      auto json_data = crow::json::load(req.body);
      if (!json_data) {
        response = ResponseHelper::make_response(400, "Invalid Json");
        return response;
      }
       std::cout << "Parsed JSON: " << json_data << std::endl;
      response = UserService::createUser(json_data);
    }catch(std::runtime_error e){
      std::cerr << e.what();
      response = ResponseHelper::make_response(500, e.what());
    }
      return response;
  });
  
  //messaging web socket
  CROW_ROUTE(app, "/chat/").websocket().onopen([&](crow::websocket::connection& conn){
    std::cout << "Received Web Socket Connection Request\n";
  })
  .onclose([&](crow::websocket::connection& conn, std::string reason){
      
    })
  .onmessage([&](crow::websocket::connection& conn, const std::string& data, bool is_binary){

    }); 




  app.port(8080).multithreaded().run();
}
