#include <iostream>
#include "../manager/connection_manager.h"

class SearchRepository{
public:
  static pqxx::result partialSearchForThreads(const std::string& target, int& err, std::string& errMsg);
  static pqxx::result partialSearchForCommunities(const std::string& target, int& err, std::string& errMsg);
  static pqxx::result partialSearchForUsers(const std::string& target, int& err, std::string& errMsg);
};
