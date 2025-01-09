#pragma once
#include "../../crow_all.h"
#include <iostream>
#include <string>
#include <unordered_map>

class LoginService{
  public:

  static crow::json::wvalue check_login(std::string& emailHash, std::string& passwordHash);

};
