#pragma once
#include <crow/json.h>

class UserService{
public:
  static crow::json::wvalue getUserWithEmail(const std::string& email);
  static crow::json::wvalue getUserWithName(const std::string& name);
  static crow::json::wvalue getUserWithUserId(const std::string& userId);
  static crow::json::wvalue getUser(const std::string& field, const std::string value);
  static crow::json::wvalue createUser(crow::json::rvalue jsonData);
};
