#include <crow.h>
#include <crow/app.h>
#include <crow/json.h>
#include <string>
#include "db/login/login_repository.h"
#include "helpers/ResponseHelper.h"

#define CROW_MAIN

int main(){
  crow::SimpleApp app;
  

  CROW_ROUTE(app, "/")([](){
    return "Hello World";
  });

  CROW_ROUTE(app, "/login/<string>/<string>")([](std::string email, std::string password){
    
    std::string error_message;
    bool login_result;
    int error_code = -1;
    int retries = 3;
    
    do{
      login_result = LoginRepository::login(email, password, error_message, error_code);
      retries--;
      if(error_code) std::cerr << error_message;
    }while(error_code && retries > 0);
    
    crow::json::wvalue body;
    body["success"] = login_result;
    
    crow::json::wvalue response = ResponseHelper::make_response(200, "OK");
    response["grant_access"] = login_result;
    return response;
  });

  app.port(8080).multithreaded().run();
}
