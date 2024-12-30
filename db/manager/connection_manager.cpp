#include "connection_manager.h"
#include <iostream>
#include <memory>
#include <strings.h>

ConnectionManager *ConnectionManager::instance = nullptr;
std::mutex ConnectionManager::instance_mutex;

ConnectionManager::~ConnectionManager() {
  std::cout << "Closing all Connections\n";
  closeConnections();
}

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

pqxx::connection& ConnectionManager::getConnection(int index){
  if(index < 0 || index > MAX_CONNECTION){
     throw std::out_of_range("Invalid connection index: " + std::to_string(index));
  }
  return *connection_pool[index];
}

void ConnectionManager::releaseConnection(int index) {
  if (index == -1) {
    return;
  }

  std::unique_lock<std::mutex> lock(free_connections_mutex);
  free_connections.push(index);
}

ConnectionManager *ConnectionManager::getInstance() {
  if (instance == nullptr) {
    std::lock_guard<std::mutex> lock(instance_mutex);
    if (instance == nullptr) {
      instance = new ConnectionManager();
    }
  }
  return instance;
}

void ConnectionManager::closeConnections() {
  for (auto& conn : connection_pool) {
    if (conn && conn->is_open()) {
      try {
        conn->close();
      } catch (const std::exception& e) {
        std::cerr << "Error closing database connection: " << e.what() << std::endl;
      }
    }
  }
}
