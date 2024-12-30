#include "routing.h"
#include <iostream>
#include <memory>
#include <mutex>

std::unordered_map<int, std::shared_ptr<crow::websocket::connection>> RoutingService::connections;
std::mutex RoutingService::connection_mutex;

void RoutingService::addConnection(const int &key, std::shared_ptr<crow::websocket::connection> conn) {
  
  std::unique_lock<std::mutex> lock(connection_mutex);
  connections[key] = conn;
  std::cout << "Added new connection for user " << key << std::endl;
}

void RoutingService::routeMessage(const int &dest, const std::string &data) {
  std::unique_lock<std::mutex> lock(connection_mutex);
  auto it = connections.find(dest);

  if (it != connections.end()) {
    try {
      it->second->send_text(data);
      std::cout << "Sent data to user " << dest << std::endl;
    } catch (std::exception &e) {
      std::cout << "Failed to send data to user " << dest << " " << e.what()
                << std::endl;
    }
  } else {
    std::cout << "User has no Connection\n";
  }
}
