#include "category.h"
#include "../../db/categories/category_repo.h"
#include "../../helpers/ResponseHelper.h"
#include <pqxx/internal/statement_parameters.hxx>
#include <string>

crow::json::wvalue CategoryService::getCategories(const std::string& filter, const std::string& value){
  int err = 0;
  std::string errMsg = "";

  pqxx::result res = CategoryRepository::getCategories(filter, value, err, errMsg);

  crow::json::wvalue data = ResponseHelper::make_response(err, errMsg);
  int i = 0;
  if(err  == 200){
    for(const auto row : res){
        data[i]["category_id"] = row[0].as<std::string>();
        data[i]["category_name"] = row[1].as<std::string>();
        i++;
    }
  }
  return data;
}
