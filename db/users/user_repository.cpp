#include "user_repository.h"
#include "../manager/connection_manager.h"
#include <exception>
#include <iostream>
#include <memory>
#include <pqxx/internal/statement_parameters.hxx>
#include <string>

pqxx::result UserRepository::getUser(const std::string &field,
                                     const std::string &value, int &error_code,
                                     std::string &error_msg) {
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

    conn.prepare("get_user", "select * from users where " + field + "= $1");
    pqxx::result res{tx.exec_prepared("get_user", value)};

    if (res.empty()) {
      error_msg = "User Not Found\n";
    } else {
      error_msg = "No Error";
    }
    
    error_code = 200;
    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return res;

  } catch (std::exception e) {
    std::cerr << "Unexpected Error Occured " << e.what();
    ConnectionManager::getInstance()->releaseConnection(conn_index);
  }
  return pqxx::result();
}

void UserRepository::addNewUser(
    const int& user_id, const std::string &name, const std::string &email,
    const std::string &dob, const std::string &bio, const std::string &password,
    const std::string &created_at, int &error_code, std::string &error_msg) {

  std::cout<<"\t User repository Adding new User to db...\n";
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
        "add_user",
        "INSERT INTO users (user_id, user_name, user_email, user_dob, "
        "user_bio, passwd, created_at) VALUES ($1, $2, $3, $4, $5, $6, $7)");

    
    tx->exec_prepared("add_user",user_id, name, email, dob, bio, password,
                     created_at);
    tx->commit();
    error_code = 201;
    error_msg = "User created successfully";
  } catch (const pqxx::sql_error &e) {
    error_code = 500;
    error_msg = "Database error: " + std::string(e.what());
    std::cerr << error_msg << std::endl;
    tx->abort();
    ConnectionManager::getInstance()->releaseConnection(conn_index);
  }

  ConnectionManager::getInstance()->releaseConnection(conn_index);
}

pqxx::result UserRepository::getUserProfilePic(const int& user_id){
  int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

  if(conn_index == -1)
    return pqxx::result();

  std::unique_ptr<pqxx::work>tx;
  try{
    pqxx::connection &conn = ConnectionManager::getInstance()->getConnection(conn_index);

    if(!conn.is_open()){
      return pqxx::result();
    }

    tx = std::make_unique<pqxx::work>(conn);

    conn.prepare("get_profile_pic", "SELECT image from profile_pics_bucket where user_id = $1");
    pqxx::result res {tx->exec_prepared("get_profile_pic", user_id)};
    tx->commit();

    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return res;
  }catch(const std::exception& e){
    std::cerr << "Unexpected Error : " << e.what() << std::endl;
    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return pqxx::result();
  }
  
}

bool UserRepository::updateUser(const std::string& field, const std::string& value, const std::string& key) {
    int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

    if (conn_index == -1) {
        std::cerr << "Update User: No Connection Available\n";
        return false;
    }

    try {
        pqxx::connection &conn = ConnectionManager::getInstance()->getConnection(conn_index);

        if (!conn.is_open()) {
            std::cerr << "Update User Error: Connection is closed\n";
            ConnectionManager::getInstance()->releaseConnection(conn_index);
            return false;
        }

        pqxx::work tx{conn};

        if (field != "profile_image") {
            conn.prepare("update_user", "UPDATE users SET " + field + " = $1 WHERE user_id = $2");
            tx.exec_prepared("update_user", value, key);
        } else {
            // Prepare and execute the update query for the profile image
            conn.prepare("update_profile_image", "UPDATE profile_pics_bucket SET image = $1 WHERE user_id = $2");
            tx.exec_prepared("update_profile_image", value, key);
        }

        // Commit the transaction
        tx.commit();

        // Release the connection
        ConnectionManager::getInstance()->releaseConnection(conn_index);
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Update User Error: " << e.what() << "\n";

        // Release the connection in case of an error
        ConnectionManager::getInstance()->releaseConnection(conn_index);
        return false;
    }
}


int UserRepository::getNextUserIdSequenceValue(){
   int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

  if(conn_index == -1)
    return 111;

  std::unique_ptr<pqxx::work>tx;
  try{
    pqxx::connection &conn = ConnectionManager::getInstance()->getConnection(conn_index);

    if(!conn.is_open()){
      return 111;
    }

    tx = std::make_unique<pqxx::work>(conn);
    pqxx::result res {tx->exec("SELECT NEXTVAL('IDSEQ')")};
    tx->commit();

    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return res[0][0].as<int>();
  }catch(const std::exception& e){
    std::cerr << "Unexpected Error : " << e.what() << std::endl;
    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return 111;
  }
  
}
