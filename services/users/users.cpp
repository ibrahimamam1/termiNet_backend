#include "users.h"
#include "../../db/users/user_repository.h"
#include "../../helpers/ResponseHelper.h"
#include <crow/json.h>
#include <iostream>
#include <string>

/*
*Takes the email send of the user and return json data will the information stored on the user
*If email is not found in database return 404 user not found 
*/
crow::json::wvalue UserService::getUser(const std::string& field, const std::string value) {
    crow::json::wvalue userData;
    int err = 0;
    std::string error_msg = "";
    pqxx::result res = UserRepository::getUser(field, value, err, error_msg);
    userData = ResponseHelper::make_response(err, error_msg);
    
    if (err == 200){
        const pqxx::row& row = res[0];  // Get the first (and only) row
        
        userData["body"]["user_id"] = row["user_id"].as<int>();
        userData["body"]["user_name"] = row["user_name"].as<std::string>();
        userData["body"]["user_email"] = row["user_email"].as<std::string>();
        userData["body"]["user_sex"] = row["user_sex"].as<std::string>();
        userData["body"]["user_bio"] = row["user_bio"].as<std::string>();
        userData["body"]["created_at"] = row["created_at"].as<std::string>();
    }
    
    return userData;
}

crow::json::wvalue UserService::getUserWithEmail(const std::string& email) {
  return getUser("user_email", email);
}

crow::json::wvalue UserService::getUserWithName(const std::string& name) {
  return getUser("user_name", name);
}

crow::json::wvalue UserService::getUserWithUserId(const std::string& userId){
  return getUser("user_id", userId);
}
crow::json::wvalue UserService::createUser(crow::json::rvalue jsonData){
  std::string name = jsonData["user_name"].s();
  std::string email = jsonData["user_email"].s();
  std::string sex = jsonData["user_sex"].s();
  std::string dob = jsonData["user_dob"].s();
  std::string bio = jsonData["user_bio"].s();
  std::string pass = jsonData["password"].s();
  std::string created_at = jsonData["created_at"].s();

  int err = -1;
  std::string message;

  UserRepository::addNewUser(name, email, sex, dob, bio, pass, created_at, err, message);

  crow::json::wvalue returnData;
  returnData = ResponseHelper::make_response(err, message);
  std::cout << message << std::endl;
  return returnData;
}

