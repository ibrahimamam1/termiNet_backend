#pragma once;
#include <crow/json.h>
class UserService{
  static crow::json::wvalue& getUserWithId(int& id);
};
