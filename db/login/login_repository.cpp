#include "login_repository.h"
#include <pqxx/pqxx>

bool LoginRepository::login(const std::string &emailHash,
                            const std::string &passwordHash,
                            std::string &message, int &error_code) {
  try {
    pqxx::connection conn("dbname=terminet user=rgb password=");
    if (!conn.is_open()) {
      message = "Failed to Connect to Database";
      error_code = 1;
      return false;
    }

    pqxx::work tx(conn);
    conn.prepare("login_check" ,"SELECT login_check($1, $2)");
    pqxx::result r{tx.exec_prepared("login_check", emailHash, passwordHash)};

    if (r.empty()) {
      message = "Invalid username or password";
      error_code = 2;
      return false;
    }

    if (r[0][0].is_null()) {
      message = "Login check result was null";
      error_code = 3;
      return false;
    }

    message = "Login successful";
    error_code = 0;

    return r[0][0].as<bool>();
  } catch (const std::exception &e) {
    message = e.what();
    error_code = 4;
    return false;
  }
}
