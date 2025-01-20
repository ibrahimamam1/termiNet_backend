#include <iostream>
#include "../manager/connection_manager.h"
#include "thread_repo.h"

int ThreadRepository::addNewThread(const std::string& title, const std::string& content, const std::string& author_id,
                                   const int& community_id, const int& parent_thread_id, std::string& errorMsg)
{
  std::cout << "\t Thread repository Adding new Thread to db...\n";
  int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

  if (conn_index == -1) {
    errorMsg = "No database connections available";
    return 500;
  }

  std::unique_ptr<pqxx::work> tx;
  try {
    pqxx::connection &conn = ConnectionManager::getInstance()->getConnection(conn_index);

    if (!conn.is_open()) {
      errorMsg = "Failed to connect to database";
      ConnectionManager::getInstance()->releaseConnection(conn_index);
      return 500;
    }

    tx = std::make_unique<pqxx::work>(conn);
    conn.prepare(
        "insert_thread",
        "INSERT INTO threads(title, content, created_at, author_id, community_id, parent_thread_id) "
        "VALUES ($1, $2, NOW(), $3, $4, $5)");

    tx->exec_prepared("insert_thread", title, content, author_id, community_id, parent_thread_id);

    tx->commit();
  } catch (const pqxx::sql_error &e) {
    errorMsg = e.what();
    if (tx) {
      tx->abort();
    }
    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return 500;
  }

  ConnectionManager::getInstance()->releaseConnection(conn_index);
  errorMsg = "Success";
  return 200;
}

pqxx::result ThreadRepository::getThreads(const std::string& filter, const std::string& filter_value, int& error_code, std::string& errorMsg) {
  int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

  if (conn_index == -1) {
    error_code = 503;
    errorMsg = "No database connections available\n";
    return pqxx::result();
  }
  try {
    pqxx::connection &conn =
        ConnectionManager::getInstance()->getConnection(conn_index);
    if (!conn.is_open()) {
      error_code = 500;
      errorMsg = "Failed to connect to database\n";
      return pqxx::result();
    }
    pqxx::work tx{conn};
    std::string query;
    
    if(filter == "none"){
      query = "SELECT * FROM threads where parent_thread_id = 0 order by created_at desc";
      pqxx::result res{tx.exec(query)};
      errorMsg = "No Error";
      error_code = 200;
      ConnectionManager::getInstance()->releaseConnection(conn_index);
      return res;
    
    }else{
      std::string query = "SELECT * FROM threads WHERE " + filter + " = $1 order by created_at desc";
      conn.prepare("get_threads_filtered", query);
      pqxx::result res{tx.exec_prepared("get_threads_filtered", filter_value)};
      errorMsg = "No Error";
      error_code = 200;
      ConnectionManager::getInstance()->releaseConnection(conn_index);
      return res;
    }
  } catch (const pqxx::sql_error &e) {
    std::cerr << "Database error: " << e.what();
    error_code = 500;
    errorMsg = "Database error: " + std::string(e.what());
    ConnectionManager::getInstance()->releaseConnection(conn_index);
  } catch (const std::exception &e) {
    std::cerr << "Unexpected Error Occurred: " << e.what();
    error_code = 500;
    errorMsg = "Unexpected error: " + std::string(e.what());
    ConnectionManager::getInstance()->releaseConnection(conn_index);
  }
  return pqxx::result();
}
