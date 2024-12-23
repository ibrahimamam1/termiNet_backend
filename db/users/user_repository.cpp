#include "user_repository.h"
#include "../manager/connection_manager.h"
#include <iostream>
#include <string>


pqxx::result UserRepository::getUser(const std::string& field, const std::string& value, int& error_code, std::string& error_msg){
  int conn_index = ConnectionManager::getInstance()->getConnectionIndex();
  
  std::unique_ptr<pqxx::connection>conn = ConnectionManager::getInstance()->getConnection(conn_index);
  if(!conn->is_open()){
    error_code = 500;
    error_msg = "Failed to connect to database\n";
    return pqxx::result();
  }
  pqxx::work tx{*conn};

  conn->prepare("get_user","select * from users where " + field + "= $1");
  pqxx::result res{tx.exec_prepared("get_user", value)};

  if(res.empty()){
    error_code = 404;
    error_msg = "User Not Found\n";
  }
  ConnectionManager::getInstance()->releaseConnection(conn_index);
  error_code = 200;
  error_msg = "No Error";
  return res;
}

void UserRepository::addNewUser(const std::string& name, const std::string& email, const std::string& sex, const std::string& dob, const std::string& bio, const std::string& password, const std::string& created_at, int& error_code, std::string& error_msg){
  int conn_index = ConnectionManager::getInstance()->getConnectionIndex();
  std::unique_ptr<pqxx::connection> conn = ConnectionManager::getInstance()->getConnection(conn_index);

  if (!conn->is_open()) {
    error_code = 500;
    error_msg = "Failed to connect to database";
    std::cerr << error_msg << std::endl;
    return;
  }

  pqxx::work tx{*conn};

  try {
    conn->prepare("add_user", "INSERT INTO users (user_name, user_email, user_sex, user_dob, user_bio, passwd, created_at) VALUES ($1, $2, $3, $4, $5, $6, $7)");
    tx.exec_prepared("add_user", name, email, sex, dob, bio, password, created_at);
    tx.commit();
    error_code = 201;
    error_msg = "User created successfully";
  } catch (const pqxx::sql_error& e) {
    error_code = 500;
    error_msg = "Database error: " + std::string(e.what());
    std::cerr << error_msg << std::endl;
    tx.abort();
  }

  ConnectionManager::getInstance()->releaseConnection(conn_index);
}
