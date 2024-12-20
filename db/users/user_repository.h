#pragma once

#include <pqxx/pqxx>

class UserRepository{
public:

  static pqxx::result getUserWithEmail(std::string& email, int& error_code);
};
