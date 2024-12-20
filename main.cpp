#include <crow.h>
#include <crow/json.h>
#include <csignal>
#include <iostream>
#include <string>
#include "services/login/login.h"
#include "services/users/users.h"
#include "helpers/ResponseHelper.h"
#include "db/manager/connection_manager.h"

#define CROW_MAIN

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
  CROW_ROUTE(app, "/users/<string>")([](std::string email){
    crow::json::wvalue response = UserService::getUserWithEmail(email);
    return response;
  }); 
  app.port(8080).multithreaded().run();
}
