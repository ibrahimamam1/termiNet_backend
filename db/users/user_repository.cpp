#include "user_repository.h"
#include "../manager/connection_manager.h"
#include <iostream>


pqxx::result UserRepository::getUserWithEmail(std::string& email, int& error_code){
  int conn_index = ConnectionManager::getInstance()->getConnectionIndex();
  
  std::unique_ptr<pqxx::connection>conn = ConnectionManager::getInstance()->getConnection(conn_index);
  if(!conn->is_open()){
    error_code = 500;
    std::cerr << "Failed to connect to database\n";
    return pqxx::result();
  }
  pqxx::work tx{*conn};

  conn->prepare("get_user","select * from users where user_email=$1");
  pqxx::result res{tx.exec_prepared("get_user", email)};

  if(res.empty()){
    error_code = 404;
    std::cerr << "User Not Found\n";
  }
  ConnectionManager::getInstance()->releaseConnection(conn_index);
  error_code = 200;
  return res;
}
