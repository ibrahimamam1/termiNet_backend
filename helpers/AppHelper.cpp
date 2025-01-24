#include "AppHelper.h"
#include <fstream>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int AppHelper::getEnvData(std::string& userName, std::string& db_name) {
    std::ifstream in("../.env");
    if (!in.is_open()) {
        std::cerr << "Failed to open .env file" << std::endl;
        return -1; // Return an error code if the file cannot be opened
    }

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            if (key == "user") {
                std::getline(iss, userName);
                // Remove any leading/trailing whitespace
                userName.erase(0, userName.find_first_not_of(' '));
                userName.erase(userName.find_last_not_of(' ') + 1);
            } else if (key == "db_name") {
                std::getline(iss, db_name);
                // Remove any leading/trailing whitespace
                db_name.erase(0, db_name.find_first_not_of(' '));
                db_name.erase(db_name.find_last_not_of(' ') + 1);
            }
        }
    }

    in.close();
    return 1;
}

