#include <iostream>
#include <pqxx/pqxx>
#include <vector>

class CommunityRepository{
public:
  static int addNewCommunity(const std::string& name, std::string& description,
                              const std::string& iconImage, const std::string& bannerImage,
                              const std::vector<int>& categories,
                              const std::string& founder, std::string& errMsg);
  static pqxx::result getCommunities(const std::string& filter, const std::string& value, int& err, std::string& errMsg);
  static int removeUserFromCommunity(const std::string& user_id, const int& community_id, std::string& errMsg);
  static int addUserToCommunity(const std::string& user_id, const int& community_id, std::string& errMsg);
};
