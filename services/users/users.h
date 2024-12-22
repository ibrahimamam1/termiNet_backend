#pragma once
#include <crow/json.h>

class UserService{
public:
  static crow::json::wvalue getUserWithEmail(std::string& email);
  static crow::json::wvalue createUser(crow::json::rvalue jsonData);
};
