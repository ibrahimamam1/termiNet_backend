#include <string>
#include <pqxx/pqxx>

class ThreadRepository{
public:
  static int addNewThread(const std::string& title, const std::string& content,
                           const std::string& created_at, const std::string& author_id,
                           const int& community_id, const int& parent_thread_id, std::string& errorMsg);

  static pqxx::result getThreads(const std::string& filter, const std::string& filter_value, int& err_code, std::string& errorMsg);
};
