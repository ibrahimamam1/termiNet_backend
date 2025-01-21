#include "community_repo.h"
#include "../manager/connection_manager.h"
#include <iostream>
#include <pqxx/internal/statement_parameters.hxx>

int CommunityRepository::addNewCommunity(const std::string& name, std::string& description,
                              const std::string& iconImage, const std::string& bannerImage,
                              const std::vector<int>& categories,
                              const std::string& founder, std::string& errMsg)
{
  std::cout << "\t Community repository Adding new Community to db...\n";
  int conn_index = ConnectionManager::getInstance()->getConnectionIndex();

  if (conn_index == -1) {
    errMsg = "No database connections available";
    return 500;
  }

  std::unique_ptr<pqxx::work> tx;
  try {
    pqxx::connection &conn = ConnectionManager::getInstance()->getConnection(conn_index);

    if (!conn.is_open()) {
      errMsg = "Failed to connect to database";
      ConnectionManager::getInstance()->releaseConnection(conn_index);
      return 500;
    }

    tx = std::make_unique<pqxx::work>(conn);

    //get unique id
    pqxx::result res{tx->exec("SELECT NEXTVAL('COMMUNITY_SEQ')")};
    int id = res[0][0].as<int>();
    std::cout << "got Unique Id: " << id << std::endl; 
    
    //insert community in community table
    conn.prepare(
        "insert_community",
        "INSERT INTO communities(community_id, name, description, icon_image, banner_image, created_at) "
        "VALUES ($1, $2, $3, $4, $5, NOW() )");

    tx->exec_prepared("insert_community",id, name, description, iconImage, bannerImage);
    
    //inser categories in community_categories
    
    conn.prepare(
      "insert_categories",
      "INSERT INTO community_categories(community_id, category_id) VALUES ($1, $2)"
    );

    for(auto& cat : categories){
      tx->exec_prepared("insert_categories", id, cat);
    }
    
    //insert founder in user_communities
    conn.prepare(
      "insert_founder",
      "INSERT INTO users_communities(user_id, community_id) VALUES($1, $2)"
    );
    tx->exec_prepared("insert_founder", founder, id);
    tx->commit();
  } catch (const pqxx::sql_error &e) {
    std::cerr << "Unexpected Error: " << e.what() << std::endl;
    errMsg = e.what();
    if (tx) {
      tx->abort();
    }
    ConnectionManager::getInstance()->releaseConnection(conn_index);
    return 500;
  }

  ConnectionManager::getInstance()->releaseConnection(conn_index);
  errMsg = "Success";
  return 200;

}

pqxx::result CommunityRepository::getCommunities(const std::string& filter, const std::string& value, int& err, std::string& errMsg){
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
    
    if(filter == "user_id"){
      conn.prepare("get_comms" ,"SELECT * FROM communities where community_id IN (select community_id from users_communities where user_id = $1)");
      pqxx::result res{tx.exec_prepared("get_comms" ,value)};
      errMsg = "No Error";
      err = 200;
      ConnectionManager::getInstance()->releaseConnection(conn_index);
      return res;
    
    }

  } catch (const pqxx::sql_error &e) {
    std::cerr << "Database error: " << e.what();
    err = 500;
    errMsg = "Database error: " + std::string(e.what());
    ConnectionManager::getInstance()->releaseConnection(conn_index);
  } catch (const std::exception &e) {
    std::cerr << "Unexpected Error Occurred: " << e.what();
    err = 500;
    errMsg = "Unexpected error: " + std::string(e.what());
    ConnectionManager::getInstance()->releaseConnection(conn_index);
  }
  return pqxx::result();

}
