#pragma once
#include <crow/json.h>
#include <string>

class ResponseHelper {
public:

  static crow::json::wvalue make_response(int statusCode);
  crow::json::wvalue& add_data(crow::json::wvalue& response, const std::string& key, const crow::json::rvalue& value);
  crow::json::wvalue& add_error(crow::json::wvalue& response, const std::string& message, const std::string& details);

};
