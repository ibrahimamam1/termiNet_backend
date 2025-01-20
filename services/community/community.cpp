#include "community.h"
#include "../../db/community/community_repo.h"
#include "../../helpers/ResponseHelper.h"
#include <string>
#include <vector>

crow::json::wvalue
CommunityService::addNewCommunity(const crow::json::rvalue &jsonData) {
  std::string name = jsonData["community_name"].s();
  std::string des = jsonData["community_description"].s();
  std::string iconImage = jsonData["icon_image"].s();
  std::string bannerImage = jsonData["banner_image"].s();
  std::string founder = jsonData["founder"].s();

  std::vector<int> categories;
  if (jsonData.has("categories")) {
    const crow::json::rvalue &categoriesJson = jsonData["categories"];
    for (const auto &category : categoriesJson) {
      categories.push_back(category.i());
    }
  }
  std::string errMsg = "";
  int err = CommunityRepository::addNewCommunity(name, des, iconImage,
                                                 bannerImage,
                                                 categories, founder, errMsg);

  crow::json::wvalue returnData = ResponseHelper::make_response(err, errMsg);
  if (err == 200)
    returnData["Status"] = "Created";
  else
    returnData["Status"] = "Failed";

  return returnData;
}
