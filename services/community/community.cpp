#include "community.h"
#include "../../db/community/community_repo.h"
#include "../../helpers/ResponseHelper.h"
#include <pqxx/internal/statement_parameters.hxx>
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
    std::cout << categoriesJson << std::endl;
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

crow::json::wvalue CommunityService::getCommunities(const std::string& filter, const std::string& value){
  
  std::cout << "Community Service: getting Communities\n";
  int err = 0;
  std::string errMsg = "";
  pqxx::result res = CommunityRepository::getCommunities(filter, value, err, errMsg);

  std::cout << "Community Service: got results\n";
  crow::json::wvalue returnData = ResponseHelper::make_response(err, errMsg);
  if(err == 200){
  std::cout << "Community Service: Good status code\n";
    int i=0;
    for(auto row : res){
      crow::json::wvalue community;
      returnData[i]["community_id"] = row[0].as<int>();
      returnData[i]["community_name"] =  row[1].as<std::string>();
      returnData[i]["community_description"] =  row[2].as<std::string>();
      returnData[i]["icon_image"] =  row[3].as<std::string>();
      returnData[i]["banner_image"] =  row[4].as<std::string>();
      returnData[i]["created_at"] =  row[5].as<std::string>();
      returnData[i]["member_count"] =  row[6].as<int>();
      i++;
    }
  }
  return returnData;
}
  
crow::json::wvalue CommunityService::removeUserFromCommunity(const std::string& user_id, const int community_id){
  std::string errMsg = "";
  int err = CommunityRepository::removeUserFromCommunity(user_id, community_id, errMsg);
  std::cout << "remove user from community msg : " << errMsg << std::endl;
  crow::json::wvalue returnData = ResponseHelper::make_response(err, errMsg);
  if(err == 200){
    returnData["Status"] = true;
  }
  else{
    returnData["Status"] = false;
  }
  return returnData;
}
