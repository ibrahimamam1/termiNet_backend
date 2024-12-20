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

std::unique_ptr<pqxx::connection> ConnectionManager::getConnection() {

  std::unique_lock<std::mutex> lock(free_connections_mutex);
  if (free_connections.empty()) {
    std::cerr << "No Connections available\n";
    return nullptr;
  }

  int index = free_connections.front();
  free_connections.pop();
  return std::move(connection_pool[index]);
}

void ConnectionManager::releaseConnection(
    std::unique_ptr<pqxx::connection> connection) {
  if (connection == nullptr) {
    return;
  }

  std::unique_lock<std::mutex> lock(free_connections_mutex);

  for (int i = 0; i < MAX_CONNECTION; i++) {
    if (connection_pool[i].get() == connection.get()) {
      free_connections.push(i);
      return;
    }
  }

  std::cerr
      << "Could not find given connection on connection pool to release \n";
}

ConnectionManager *ConnectionManager::getInstance() {
  if (instance == nullptr)
    instance = new ConnectionManager();
  return instance;
}
