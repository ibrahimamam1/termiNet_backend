#include "../../crow_all.h"
#include <memory>
#include <mutex>
#include <unordered_map>

class RoutingService{
  static std::unordered_map<int, std::shared_ptr<crow::websocket::connection>> connections;  static std::mutex connection_mutex;

  public:
  static void addConnection(const int &key, std::shared_ptr<crow::websocket::connection> conn);
  static void removeConnection(crow::websocket::connection* conn);
  static void routeMessage(const int& src, const int& dest, const std::string& data);
  static void sendUndeliveredMessages(const int& key);
};
