#include <iostream>
#include <pqxx/pqxx>

class CategoryRepository{
public:
  static pqxx::result getCategories(const std::string& filter, const std::string& value, int& err, std::string& errMsg);
};
