#include "category_repo.h"
#include "../manager/connection_manager.h"

pqxx::result CategoryRepository::getCategories(const std::string& filter, const std::string& value, int& err, std::string& errMsg){
  int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

  if (conn_index == -1) {
    err = 503;
    errMsg = "No database connections available\n";
    return pqxx::result();
  }
  try {
    pqxx::connection &conn =
        ConnectionManager::getInstance()->getConnection(conn_index);
    if (!conn.is_open()) {
      err = 500;
      errMsg = "Failed to connect to database\n";
      return pqxx::result();
    }
    pqxx::work tx{conn};
    std::string query;
    
    if(filter == "none"){
      query = "SELECT * FROM categories";
      pqxx::result res{tx.exec(query)};
      errMsg = "No Error";
      err = 200;
      ConnectionManager::getInstance()->releaseConnection(conn_index);
      return res;
    
    }else{
      std::string query = "SELECT * FROM categories WHERE " + filter + " = $1";
      conn.prepare("get_categories_filtered", query);
      pqxx::result res{tx.exec_prepared("get_threads_filtered", value)};
      errMsg = "No Error";
      err = 200;
      ConnectionManager::getInstance()->releaseConnection(conn_index);
      return res;
    }
  } catch (std::exception e) {
    err = 500;
    errMsg = e.what();
    ConnectionManager::getInstance()->releaseConnection(conn_index);
  }
  return pqxx::result();

}
