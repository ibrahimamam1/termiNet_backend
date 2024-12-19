
#include <pqxx/pqxx>
#include <string>

class LoginRepository{
  public: 
  static bool login(const std::string &email, const std::string &password, std::string& message, int &error_code);

};
