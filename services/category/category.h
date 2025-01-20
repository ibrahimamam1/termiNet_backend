#include<iostream>
#include "../../crow_all.h"

class CategoryService{
public:
  static crow::json::wvalue getCategories(const std::string& filter, const std::string& value);
};
