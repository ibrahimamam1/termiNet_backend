#include "login_repository.h"
#include "../manager/connection_manager.h"
#include <memory>
#include <pqxx/internal/statement_parameters.hxx>


pqxx::result LoginRepository::login(const std::string &emailHash,
                            const std::string &passwordHash,
                            std::string &message, int &error_code) {
  try {
    int conn_index = ConnectionManager::getInstance()->getConnectionIndex();
    std::unique_ptr<pqxx::connection> conn = ConnectionManager::getInstance()->getConnection(conn_index);
    if (!conn->is_open()) {
      message = "Failed to Connect to Database";
      error_code = 500;
      return pqxx::result();
    }

    pqxx::work tx(*conn);
    conn->prepare("login_check" ,"SELECT login_check($1, $2)");
    pqxx::result r{tx.exec_prepared("login_check", emailHash, passwordHash)};

    if (r.empty()) {
      message = "Invalid username or password";
      error_code = 404;
      return r;
    }

    message = "Login successful";
    error_code = 200;
    
    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return r;
  } catch (const std::exception &e) {
    message = e.what();
    error_code = 500;
    return pqxx::result();
  }
}
