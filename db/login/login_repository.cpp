#include "login_repository.h"
#include "../manager/connection_manager.h"

pqxx::result LoginRepository::login(const std::string &emailHash,
                                    const std::string &passwordHash,
                                    std::string &message, int &error_code) {
  int conn_index = -1;
  try {
    conn_index = ConnectionManager::getInstance()->getConnectionIndex();

    if (conn_index == -1) {
      message = "No database connections available";
      error_code = 500;
      return pqxx::result();
    }

    pqxx::connection &conn =
        ConnectionManager::getInstance()->getConnection(conn_index);
    if (!conn.is_open()) {
      message = "Failed to Connect to Database";
      error_code = 500;
      ConnectionManager::getInstance()->releaseConnection(conn_index);
      return pqxx::result();
    }

    pqxx::work tx(conn);
    conn.prepare("login_check", "SELECT login_check($1, $2)");
    pqxx::result r{tx.exec_prepared("login_check", emailHash, passwordHash)};
    tx.commit();

    if (r.empty()) {
      message = "Invalid username or password";
      error_code = 401;
      ConnectionManager::getInstance()->releaseConnection(conn_index);
      return r;
    }

    message = "No Error";
    error_code = 200;

    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return r;
  } catch (const pqxx::sql_error &e) {
    message = "Database error: " + std::string(e.what());
    error_code = 500;
    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return pqxx::result();
  } catch (const std::exception &e) {
    message = "An unexpected error occurred: " + std::string(e.what());
    error_code = 500;
    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return pqxx::result();
  }
}
