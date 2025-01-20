#include "community_repo.h"
#include "../manager/connection_manager.h"
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
    std::cout << "got Unique Id" << std::endl; 
    
    //insert community in community table
    conn.prepare(
        "insert_community",
        "INSERT INTO communities(id, name, description, icon_image, banner_image, created_at) "
        "VALUES ($1, $2, $3, $4, $5, NOW() )");

    tx->exec_prepared("insert_community",id, name, description, iconImage, bannerImage);
    std::cout << "Inserted Community" << std::endl; 
    
    //inser categories in community_categories
    
    conn.prepare(
      "insert_categories",
      "INSERT INTO community_category(community_id, category_id) VALUES ($1, $2)"
    );

    for(auto& cat : categories){
      tx->exec_prepared("insert_categories", id, cat);
    }
    std::cout << "Inserted Categories" << std::endl; 
    
    //insert founder in user_communities
    conn.prepare(
      "insert_founder",
      "INSERT INTO users_communities(user_id, community_id) VALUES($1, $2)"
    );
    tx->exec_prepared("insert_founder", founder, id);
    std::cout << "Inserted Founder" << std::endl; 
    tx->commit();
  } catch (const pqxx::sql_error &e) {
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
