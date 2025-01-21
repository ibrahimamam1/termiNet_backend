#include "../../crow_all.h"
#include <string>

class CommunityService{
public:
  static crow::json::wvalue addNewCommunity(const crow::json::rvalue& jsonData);
  static crow::json::wvalue getCommunities(const std::string& filter, const std::string& value);
};
