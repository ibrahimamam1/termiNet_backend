#include "login.h"
#include "../../db/login/login_repository.h"

bool LoginService::check_login(std::string& emailHash, std::string& passwordHash, int& error_code){ 
    
  std::string error_message;
  bool login_result;
  int retries = 3;
    
    do{
      login_result = LoginRepository::login(emailHash, passwordHash, error_message, error_code);
      retries--;
      if(error_code) std::cerr << error_message;
    }while(error_code && retries > 0);
    return login_result;
}
