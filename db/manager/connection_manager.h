#pragma once
#include "pqxx/pqxx"
#include <bitset>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#define MAX_CONNECTION 100

class ConnectionManager {
private:
  ConnectionManager() = default;
  static ConnectionManager *instance;
  std::vector<std::unique_ptr<pqxx::connection>> connection_pool;
  std::queue<int> free_connections;
  std::mutex free_connections_mutex;

public:
  bool init(std::string connectionString);
  int getConnectionIndex();
  std::unique_ptr<pqxx::connection>getConnection(int index);
  void releaseConnection(int index);
  static ConnectionManager *getInstance();
  void closeConnections();
  ~ConnectionManager();
};
