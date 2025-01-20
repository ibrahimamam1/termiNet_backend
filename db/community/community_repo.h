#include <iostream>
#include <vector>

class CommunityRepository{
public:
  static int addNewCommunity(const std::string& name, std::string& description,
                              const std::string& iconImage, const std::string& bannerImage,
                              const std::vector<int>& categories,
                              const std::string& founder, std::string& errMsg);
};
