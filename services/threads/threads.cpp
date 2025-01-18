#include "threads.h"
#include "../../db/threads/thread_repo.h"
#include "../../helpers/ResponseHelper.h"

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
