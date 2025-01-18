#include "../../crow_all.h"
class ThreadService{
public:
  static crow::json::wvalue addNewThread(const crow::json::rvalue& jsonData);
  static crow::json::wvalue getThreads(const std::string& filter, const std::string& value);
};
