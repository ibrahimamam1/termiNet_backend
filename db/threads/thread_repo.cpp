#include <iostream>
#include "../manager/connection_manager.h"
#include "thread_repo.h"

int ThreadRepository::addNewThread(const std::string& title, const std::string& content,
                           const std::string& created_at, const std::string& author_id,
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
        "VALUES ($1, $2, $3, $4, $5, $6)");

    tx->exec_prepared("insert_thread", title, content, created_at, author_id, community_id, parent_thread_id);

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
