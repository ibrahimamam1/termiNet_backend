#include "threads.h"
#include "../../db/threads/thread_repo.h"
#include "../../helpers/ResponseHelper.h"
#include <pqxx/internal/statement_parameters.hxx>

crow::json::wvalue ThreadService::addNewThread(const crow::json::rvalue& jsonData){
  std::string title = jsonData["title"].s();
  std::string content = jsonData["content"].s();
  std::string author_id = jsonData["author_id"].s();
  int community_id = jsonData["community_id"].i();
  int parentThreadId = jsonData["parent_thread_id"].i();
  std::string createdAt = jsonData["created_at"].s();

  std::string errorMsg = "";
  int err = ThreadRepository::addNewThread(title, content, createdAt, author_id, community_id, parentThreadId, errorMsg);

  crow::json::wvalue response = ResponseHelper::make_response(err, errorMsg);
  return response;
}
crow::json::wvalue ThreadService::getThreads(const std::string& filter, const std::string& value){
  int err = 0;
  std::string errMsg = "";
  
  pqxx::result res = ThreadRepository::getThreads(filter, value, err, errMsg);
  crow::json::wvalue data = ResponseHelper::make_response(err, errMsg);
  
  int i=0;
  for(auto row : res){
    data[i]["thread_id"] = row["thread_id"].as<int>();
    data[i]["title"] = row["title"].as<std::string>();
    data[i]["content"] = row["content"].as<std::string>();
    data[i]["created_at"] = row["created_at"].as<std::string>();
    data[i]["author_id"] = row["author_id"].as<std::string>();
    data[i]["community_id"] = row["community_id"].as<int>();
    data[i]["parent_thead_id"] = row["parent_thread_id"].as<int>();
    i++;
  }
  return data;
}
