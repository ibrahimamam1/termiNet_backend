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

void RoutingService::routeMessage(const int& src, const int &dest, const std::string &data) {
  std::unique_lock<std::mutex> lock(connection_mutex);
  auto it = connections.find(dest);

  if (it != connections.end()) {
    try {
      crow::json::wvalue payload;
      payload["sender_id"] = src;
      payload["message"] = data;
      
      std::string str = payload.dump();
      it->second->send_text(str);
      
      std::cout << "Sent data to user " << dest << std::endl;
    } catch (std::exception &e) {
      std::cout << "Failed to send data to user " << dest << " " << e.what()
                << std::endl;
    }
  } else {
    std::cout << "User has no Connection\n";
  }
}

void RoutingService::removeConnection(crow::websocket::connection* conn) {
    std::unique_lock<std::mutex> lock(connection_mutex);
    
    // Find and remove the connection that matches the pointer
    for (auto it = connections.begin(); it != connections.end(); ) {
        if (it->second.get() == conn) {
            std::cout << "Removing connection for user " << it->first << std::endl;
            it = connections.erase(it);
        } else {
            ++it;
        }
    }
}
