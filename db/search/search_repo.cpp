#include "search_repo.h"


pqxx::result SearchRepository::partialSearchForThreads(const std::string& target, int& err, std::string& errMsg) {
    int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

    if (conn_index == -1) {
        err = 503;
        errMsg = "No database connections available\n";
        return pqxx::result();
    }

    try {
        pqxx::connection& conn = ConnectionManager::getInstance()->getConnection(conn_index);
        if (!conn.is_open()) {
            err = 500;
            errMsg = "Failed to connect to database\n";
            return pqxx::result();
        }

        pqxx::work tx{conn};

        // Prepare the SQL statement for partial search
        conn.prepare("partial_thread_search", 
            "SELECT * FROM threads WHERE content LIKE '%' || $1 || '%'");

        // Execute the prepared statement with the target parameter
        pqxx::result res{tx.exec_prepared("partial_thread_search", target)};

        err = 200;
        errMsg = "No Error";
        ConnectionManager::getInstance()->releaseConnection(conn_index);
        return res;
    } catch (const std::exception& e) {
        err = 500;
        errMsg = e.what();
        std::cerr << "Unexpected Error Occurred: " << e.what() << std::endl;
        ConnectionManager::getInstance()->releaseConnection(conn_index);
    }

    return pqxx::result();
}
pqxx::result SearchRepository::partialSearchForCommunities(const std::string& target, int& err, std::string& errMsg) {
    int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

    if (conn_index == -1) {
        err = 503;
        errMsg = "No database connections available\n";
        return pqxx::result();
    }

    try {
        pqxx::connection& conn = ConnectionManager::getInstance()->getConnection(conn_index);
        if (!conn.is_open()) {
            err = 500;
            errMsg = "Failed to connect to database\n";
            return pqxx::result();
        }

        pqxx::work tx{conn};

        // Prepare the SQL statement for partial search
        conn.prepare("partial_community_search", 
            "SELECT * FROM communities WHERE name LIKE '%' || $1 || '%'");

        // Execute the prepared statement with the target parameter
        pqxx::result res{tx.exec_prepared("partial_community_search", target)};

        err = 200;
        errMsg = "No Error";
        ConnectionManager::getInstance()->releaseConnection(conn_index);
        return res;
    } catch (const std::exception& e) {
        err = 500;
        errMsg = e.what();
        std::cerr << "Unexpected Error Occurred: " << e.what() << std::endl;
        ConnectionManager::getInstance()->releaseConnection(conn_index);
    }

    return pqxx::result();
}

pqxx::result SearchRepository::partialSearchForUsers(const std::string& target, int& err, std::string& errMsg) {
    int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

    if (conn_index == -1) {
        err = 503;
        errMsg = "No database connections available\n";
        return pqxx::result();
    }

    try {
        pqxx::connection& conn = ConnectionManager::getInstance()->getConnection(conn_index);
        if (!conn.is_open()) {
            err = 500;
            errMsg = "Failed to connect to database\n";
            return pqxx::result();
        }

        pqxx::work tx{conn};

        // Prepare the SQL statement for partial search
        conn.prepare("partial_user_search", 
            "SELECT * FROM users WHERE user_name LIKE '%' || $1 || '%'");

        // Execute the prepared statement with the target parameter
        pqxx::result res{tx.exec_prepared("partial_user_search", target)};

        err = 200;
        errMsg = "No Error";
        ConnectionManager::getInstance()->releaseConnection(conn_index);
        return res;
    } catch (const std::exception& e) {
        err = 500;
        errMsg = e.what();
        std::cerr << "Unexpected Error Occurred: " << e.what() << std::endl;
        ConnectionManager::getInstance()->releaseConnection(conn_index);
    }

    return pqxx::result();
}
