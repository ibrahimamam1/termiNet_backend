#pragma once
#include <crow/json.h>
#include <string>

class ResponseHelper {
public:
  // Generic  response
  static crow::json::wvalue make_response(int statusCode, const std::string& status);

};
