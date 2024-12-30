#pragma once
#include "../crow_all.h"
#include <string>

class ResponseHelper {
public:

  static crow::json::wvalue make_response(int statusCode, std::string message);

};
