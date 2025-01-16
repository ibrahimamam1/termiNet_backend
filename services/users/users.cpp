#include "users.h"
#include "../../db/users/user_repository.h"
#include "../../helpers/ResponseHelper.h"
#include <iostream>
#include <pqxx/internal/statement_parameters.hxx>
#include <string>

bool UserService::userNameTaken(const std::string& user_name) {
    std::cout << "\tChecking if userName is taken\n";
    int error_code = -1;
    std::string error_msg = "";
    pqxx::result res = UserRepository::getUser("user_name", user_name, error_code, error_msg);

    return !res.empty();
}

bool UserService::emailAlreadyExist(const std::string& user_email) {
    int error_code = -1;
    std::string error_msg = "";
    pqxx::result res = UserRepository::getUser("user_email", user_email, error_code, error_msg);

    return !res.empty();
}

crow::json::wvalue UserService::getUser(const std::string& field, const std::string value) {
    crow::json::wvalue userData;
    int err = 0;
    std::string error_msg = "";
    pqxx::result res = UserRepository::getUser(field, value, err, error_msg);
    userData = ResponseHelper::make_response(err, error_msg);
    
    std::string user_id;
    if (error_msg == "No Error"){
        const pqxx::row& row = res[0];  // Get the first (and only) row
        
        userData["body"]["user_id"] = row["user_id"].as<std::string>();
        user_id = row["user_id"].as<std::string>();
        userData["body"]["user_name"] = row["user_name"].as<std::string>();
        userData["body"]["user_email"] = row["user_email"].as<std::string>();
        userData["body"]["user_bio"] = row["user_bio"].as<std::string>();
        userData["body"]["created_at"] = row["created_at"].as<std::string>();
    
    // Get profile pic
        pqxx::result profile_pic = UserRepository::getUserProfilePic(user_id);
        if (!profile_pic.empty()) {
            std::string base64_image = profile_pic[0]["image"].c_str();
            
            
            // Add to response
            userData["body"]["profile_image"] = base64_image;
        } else {
            userData["body"]["profile_image"] = "";
        }
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
  std::cout << "\tUser Service Creating User...\n";
  std::string name = jsonData["user_name"].s();
  std::string email = jsonData["user_email"].s();
  std::string dob = jsonData["user_dob"].s();
  std::string bio = jsonData["user_bio"].s();
  std::string pass = jsonData["password"].s();
  std::string created_at = jsonData["created_at"].s();
  std::string method = jsonData["method"].s();
  std::string user_id;

  std::cout << "\tExtracted user data from json\n";
  int err = -1;
  std::string message;
  
  if(userNameTaken(name)){
    std::cout <<"\tUser Name Already taken. returning";
    crow::json::wvalue returnData = ResponseHelper::make_response(200, "UserName Already Taken");
    returnData["Status"] = "Failed";
    return returnData;
  }
  
  if(method == "Email"){
    if(emailAlreadyExist(email)){
      std::cout <<"\tEmail Already Exists. returning\n";
      crow::json::wvalue returnData = ResponseHelper::make_response(200, "EmailAlreadyExist");
      returnData["Status"] = "Failed";
      return returnData;
    }
    
    //generate unique id
    int idSequence = UserRepository::getNextUserIdSequenceValue();
    int unique_id = time(NULL) + idSequence;
    user_id = std::to_string(unique_id);
  }
  else if(method == "Google"){
    user_id = jsonData["user_id"].s(); 
  }


  UserRepository::addNewUser(user_id, name, email, dob, bio, pass, created_at, err, message);
  std::cout<<"\tUser Created Successfully. returning\n";
  crow::json::wvalue returnData;
  returnData = ResponseHelper::make_response(err, message);
  std::cout << message << std::endl;
  return returnData;
}

//update user functions
crow::json::wvalue UserService::updateUser(const std::string& field, const std::string& new_value, const std::string& key){ 
  bool success = UserRepository::updateUser(field, new_value, key);
  
  crow::json::wvalue returnData;
  if(success)
    returnData = ResponseHelper::make_response(200, "Data Updated Succesfully");
  else 
    returnData = ResponseHelper::make_response(500, "Internal Server Error");

  return returnData;
}
