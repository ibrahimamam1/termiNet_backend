#include "../../crow_all.h"
class ThreadService{
public:
  static crow::json::wvalue addNewThread(const crow::json::rvalue& jsonData);
};
