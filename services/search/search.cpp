#include "search.h"
#include "../../db/search/search_repo.h"
#include <pqxx/pqxx>

crow::json::wvalue SearchService::searchFor(const std::string& target) {
    int err = 0;
    std::string errMsg = "";
    crow::json::wvalue returnData;

    // Search in threads
    pqxx::result threadRes = SearchRepository::partialSearchForThreads(target, err, errMsg);
    if (err != 200) {
        returnData["Threads_Found"] = false;
    } else {
        returnData["Threads_Found"] = true;
        crow::json::wvalue threads;
        int i = 0;
        for (auto row : threadRes) {
            threads[i]["thread_id"] = row[0].as<int>();
            threads[i]["thread_title"] = row[1].as<std::string>();
            threads[i]["thread_content"] = row[2].as<std::string>();
            threads[i]["created_at"] = row[3].as<std::string>();
            threads[i]["author_id"] = row[4].as<std::string>();
            threads[i]["community_id"] = row[5].as<int>();
            threads[i]["parent_thread_id"] = row[6].as<int>();
            threads[i]["comment_count"] = row[7].as<int>();
            i++;
        }
        returnData["Threads"] = std::move(threads);
    }

    // Search in communities
    errMsg = "";
    err = 0;
    pqxx::result commRes = SearchRepository::partialSearchForCommunities(target, err, errMsg);
    if (err != 200) {
        returnData["Communities_Found"] = false;
    } else {
        returnData["Communities_Found"] = true;
        crow::json::wvalue comms;
        int i = 0;
        for (auto row : commRes) {
            comms[i]["community_id"] = row[0].as<int>();
            comms[i]["community_name"] = row[1].as<std::string>();
            comms[i]["community_description"] = row[2].as<std::string>();
            comms[i]["icon_image"] = row[3].as<std::string>();
            comms[i]["banner_image"] = row[4].as<std::string>();
            comms[i]["created_at"] = row[5].as<std::string>(); // Assuming this is a timestamp string
            comms[i]["member_count"] = row[6].as<int>();
            i++;
        }
        returnData["Communities"] = std::move(comms);
    }

    // Search in users
    errMsg = "";
    err = 0;
    pqxx::result userRes = SearchRepository::partialSearchForUsers(target, err, errMsg);
    if (err != 200) {
        returnData["Users_Found"] = false;
    } else {
        returnData["Users_Found"] = true;
        crow::json::wvalue users;
        int i = 0;
        for (auto row : userRes) {
            users[i]["user_id"] = row[0].as<std::string>();
            users[i]["user_name"] = row[1].as<std::string>();
            users[i]["user_email"] = row[2].as<std::string>();
            users[i]["user_dob"] = row[3].as<std::string>();
            users[i]["user_bio"] = row[4].as<std::string>();
            users[i]["created_at"] = row[5].as<std::string>(); // Assuming this is a timestamp string
            users[i]["profile_picture"] = row[6].as<std::string>();
            i++;
        }
        returnData["Users"] = std::move(users);
    }

    return returnData;
}
