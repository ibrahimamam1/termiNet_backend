#include "message_repository.h"
#include "../manager/connection_manager.h"

void MessageRepository::addNewUndelivered(const int& src, const int& dest, const std::string& data, int& error_code, std::string& error_msg){

  int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

  if (conn_index == -1) {
    error_code = 503;
    error_msg = "No database connections available";
    return;
  }
  
  std::unique_ptr<pqxx::work> tx;
  try {
    pqxx::connection &conn = ConnectionManager::getInstance()->getConnection(conn_index);

    if (!conn.is_open()) {
      error_code = 500;
      error_msg = "Failed to connect to database";
      std::cerr << error_msg << std::endl;
      ConnectionManager::getInstance()->releaseConnection(conn_index);
      return;
    }

    tx = std::make_unique<pqxx::work>(conn); 

    conn.prepare(
        "add_undelivered",
        "INSERT INTO undelivered_messages (sender_id, receiver_id, sent_at, content) VALUES ($1, $2, CURRENT_TIMESTAMP, $3)");
    
    tx->exec_prepared("add_undelivered", src, dest, data);
    tx->commit();
    error_code = 201;
    error_msg = "Undeliverd added successfully";
  } catch (const pqxx::sql_error &e) {
    error_code = 500;
    error_msg = "Database error: " + std::string(e.what());
    std::cerr << error_msg << std::endl;
    tx->abort();
    ConnectionManager::getInstance()->releaseConnection(conn_index);
  }

  ConnectionManager::getInstance()->releaseConnection(conn_index);

}
void MessageRepository::removeMessage(const int& message_id, int& error_code, std::string& error_msg){
  
  int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

  if (conn_index == -1) {
    error_code = 503;
    error_msg = "No database connections available";
    return;
  }
  
  std::unique_ptr<pqxx::work> tx;
  try {
    pqxx::connection &conn = ConnectionManager::getInstance()->getConnection(conn_index);

    if (!conn.is_open()) {
      error_code = 500;
      error_msg = "Failed to connect to database";
      std::cerr << error_msg << std::endl;
      ConnectionManager::getInstance()->releaseConnection(conn_index);
      return;
    }

    tx = std::make_unique<pqxx::work>(conn); 

    conn.prepare(
        "remove_undelivered",
        "DELETE FROM undelivered_messages where message_id =  $1");
    
    tx->exec_prepared("remove_undelivered", message_id);
    tx->commit();
    error_code = 201;
    error_msg = "Undeliverd removed successfully";
  } catch (const pqxx::sql_error &e) {
    error_code = 500;
    error_msg = "Database error: " + std::string(e.what());
    std::cerr << error_msg << std::endl;
    tx->abort();
    ConnectionManager::getInstance()->releaseConnection(conn_index);
  }

  ConnectionManager::getInstance()->releaseConnection(conn_index);

}
pqxx::result MessageRepository::getAllUndelivered(const int& key, int& error_code, std::string& error_msg){

  int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

  if (conn_index == -1) {
    error_code = 503;
    error_msg = "No database connections available\n";
    return pqxx::result();
  }
  try {
    pqxx::connection &conn =
        ConnectionManager::getInstance()->getConnection(conn_index);
    if (!conn.is_open()) {
      error_code = 500;
      error_msg = "Failed to connect to database\n";
      return pqxx::result();
    }
    pqxx::work tx{conn};

    conn.prepare("get_messages", "select * from undelivered_messages where receiver_id = $1");
    pqxx::result res{tx.exec_prepared("get_messages", key)};

    if (res.empty()) {
      error_code = 404;
      error_msg = "User Not Found\n";
    } else {
      error_code = 200;
      error_msg = "No Error";
    }

    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return res;

  } catch (std::exception e) {
    std::cerr << "Unexpected Error Occured " << e.what();
    ConnectionManager::getInstance()->releaseConnection(conn_index);
  }
  return pqxx::result();

}
