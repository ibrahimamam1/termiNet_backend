#include "../../crow_all.h"

class CommunityService{
public:
  static crow::json::wvalue addNewCommunity(const crow::json::rvalue& jsonData);
};
