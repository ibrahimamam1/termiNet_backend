#include <iostream>
#include "../../crow_all.h"

class SearchService{
public:
  static crow::json::wvalue searchFor(const std::string& target);
};
