#include <iostream>
#include "pqxx/pqxx"

class MessageRepository{
public:
  static void addNewUndelivered(const int& src, const int& dest, const std::string& data, int& err, std::string& err_msg);
  static void removeMessage(const int& message_id, int& err_code, std::string& err_msg);
  static pqxx::result getAllUndelivered(const int& key, int& error_code, std::string& error_msg);
};
