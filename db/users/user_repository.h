#pragma once

#include <pqxx/pqxx>

class UserRepository{
  static pqxx::result& getUserWithEmail(std::string& email, int& error_code);
};
