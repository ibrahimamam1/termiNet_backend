#include "login.h"
#include "../../db/login/login_repository.h"
#include "../../helpers/ResponseHelper.h"

crow::json::wvalue LoginService::check_login(std::string& emailHash, std::string& passwordHash){ 
    
  int error_code = 0;
  std::string error_message;
  pqxx::result login_result;

  int retries = 3;
  do{
      login_result = LoginRepository::login(emailHash, passwordHash, error_message, error_code);
      retries--;
      if(error_code >= 500) std::cerr << error_message;
      if(error_code == 200) break;
    }while(retries > 0);
    
  crow::json::wvalue jsonData = ResponseHelper::make_response(error_code, error_message);
  if(error_code == 200){
    jsonData["body"]["Grant Access"] = login_result[0][0].as<bool>();
  }

  return jsonData;

}
