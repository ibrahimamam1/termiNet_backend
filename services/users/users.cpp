#include "users.h"
#include "../../db/users/user_repository.h"
#include "../../helpers/ResponseHelper.h"
#include <crow/json.h>

crow::json::wvalue UserService::getUserWithEmail(std::string& email) {
    crow::json::wvalue userData;
    int err = 0;
    pqxx::result res = UserRepository::getUserWithEmail(email, err);
    userData = ResponseHelper::make_response(err);
    
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

