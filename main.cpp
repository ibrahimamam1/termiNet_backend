#include <crow.h>
#include <crow/json.h>
#include <iostream>
#include <string>
#include "services/login/login.h"
#include "helpers/ResponseHelper.h"
#include "db/manager/connection_manager.h"

#define CROW_MAIN

int main(){
  crow::SimpleApp app;

  //initialise database connection
  ConnectionManager::getInstance()->init("dbname=terminet user=rgb password=");

  CROW_ROUTE(app, "/")([](){
    return "Hello World";
  });
  
  // handle login check
  CROW_ROUTE(app, "/login/<string>/<string>")([](std::string email, std::string password){ 
    int error_code = 0;
    bool login_result = LoginService::check_login(email, password, error_code);
    crow::json::wvalue response = ResponseHelper::make_response(200, "OK");
    response["grant_access"] = login_result;
    return response;
  });
  
  //handle getting a user information
  /*CROW_ROUTE(app, "/users/<int>")([](int id){
    
  }); */
  app.port(8080).multithreaded().run();
}
