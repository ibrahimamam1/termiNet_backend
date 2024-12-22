#include "ResponseHelper.h"
#include <crow/json.h>

crow::json::wvalue ResponseHelper::make_response(int statusCode, std::string message) {
  crow::json::wvalue response;
  response["StatusCode"] = statusCode;

  // 2xx Success
  if (statusCode == 200)
    response["Status"] = "OK";
  else if (statusCode == 201)
    response["Status"] = "Created";
  else if (statusCode == 202)
    response["Status"] = "Accepted";
  else if (statusCode == 204)
    response["Status"] = "No Content";

  // 3xx Redirection
  else if (statusCode == 301)
    response["Status"] = "Moved Permanently";
  else if (statusCode == 302)
    response["Status"] = "Found";
  else if (statusCode == 304)
    response["Status"] = "Not Modified";

  // 4xx Client Errors
  else if (statusCode == 400)
    response["Status"] = "Bad Request";
  else if (statusCode == 401)
    response["Status"] = "Unauthorized";
  else if (statusCode == 403)
    response["Status"] = "Forbidden";
  else if (statusCode == 404)
    response["Status"] = "Not Found";
  else if (statusCode == 405)
    response["Status"] = "Method Not Allowed";
  else if (statusCode == 409)
    response["Status"] = "Conflict";
  else if (statusCode == 429)
    response["Status"] = "Too Many Requests";

  // 5xx Server Errors
  else if (statusCode == 500)
    response["Status"] = "Internal Server Error";
  else if (statusCode == 501)
    response["Status"] = "Not Implemented";
  else if (statusCode == 502)
    response["Status"] = "Bad Gateway";
  else if (statusCode == 503)
    response["Status"] = "Service Unavailable";
  else if (statusCode == 504)
    response["Status"] = "Gateway Timeout";

  // Unknown status code
  else
    response["Status"] = "Unknown Status Code";

  response["Message"] = message;
  return response;
}


