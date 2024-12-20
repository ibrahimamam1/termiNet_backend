#include "connection_manager.h"
#include <iostream>
#include <memory>

ConnectionManager *ConnectionManager::instance = nullptr;

ConnectionManager::~ConnectionManager() {}

bool ConnectionManager::init(std::string connectionString) {

  for (int i = 0; i < MAX_CONNECTION; ++i) {
    connection_pool.push_back(
        std::make_unique<pqxx::connection>(connectionString));
    if (!connection_pool.back()->is_open()) {
      std::cerr << "Failed to initialize connection pool at index " << i
                << std::endl;
      return false;
    }
    free_connections.push(i);
  }
  return true;
}

int ConnectionManager::getConnectionIndex() {

  std::unique_lock<std::mutex> lock(free_connections_mutex);
  if (free_connections.empty()) {
    std::cerr << "No Connections available\n";
    return -1;
  }

  int index = free_connections.front();
  free_connections.pop();
  return index;
}

std::unique_ptr<pqxx::connection> ConnectionManager::getConnection(int index){
  return std::move(connection_pool[index]);
}

void ConnectionManager::releaseConnection(int index) {
  if (index == -1) {
    return;
  }

  std::unique_lock<std::mutex> lock(free_connections_mutex);
  free_connections.push(index);
}

ConnectionManager *ConnectionManager::getInstance() {
  if (instance == nullptr)
    instance = new ConnectionManager();
  return instance;
}
void ConnectionManager::closeConnections(){
  for(int i=0; i<MAX_CONNECTION; i++){
    connection_pool[i]->close();
  }
}
