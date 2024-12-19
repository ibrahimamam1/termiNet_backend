#include "ResponseHelper.h"
#include <crow/json.h>

crow::json::wvalue ResponseHelper::make_response(int statusCode, const std::string& status){
    crow::json::wvalue response;
    response["StatusCode"] = statusCode;
    response["Status"] = status;
    return response;
}
