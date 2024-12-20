#pragma once
#include <crow/json.h>

class UserService{
public:
  static crow::json::wvalue getUserWithEmail(std::string& email);
};
