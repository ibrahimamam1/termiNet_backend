#include <string>

class ThreadRepository{
public:
  static int addNewThread(const std::string& title, const std::string& content,
                           const std::string& created_at, const std::string& author_id,
                           const int& community_id, const int& parent_thread_id, std::string& errorMsg);
};
