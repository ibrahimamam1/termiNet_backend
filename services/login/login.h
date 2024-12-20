#pragma once
#include <iostream>
#include <string>

class LoginService{
  public:

  static bool check_login(std::string& emailHash, std::string& passwordHash, int& error_code);
};
