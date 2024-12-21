#pragma once

#include <pqxx/pqxx>
#include <string>

class UserRepository{
public:

  static pqxx::result getUserWithEmail(std::string& email, int& error_code);
  static void addNewUser(const std::string& name, const std::string& email, const std::string& sex, const std::string& dob, const std::string& bio, const std::string& password, const std::string& created_at, int& error_code, std::string& error_msg);

};
