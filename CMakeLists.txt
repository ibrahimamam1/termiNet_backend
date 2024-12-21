cmake_minimum_required(VERSION 3.10)
project(terminet)

set(CMAKE_CXX_STANDARD 20)

# Find Pqxx
find_package(PostgreSQL REQUIRED)
if(NOT PostgreSQL_FOUND)
    message(FATAL_ERROR "PostgreSQL development files are required to compile this project")
endif()

# Source files for the app helpers
file(GLOB APP_HELPER_SOURCES "helpers/*.cpp")
add_library(app_helper STATIC ${APP_HELPER_SOURCES})

# Source files for Connection Manager
file(GLOB CONNECTION_MANAGER_SOURCES "db/manager/*.cpp")
add_library(connection_manager STATIC ${CONNECTION_MANAGER_SOURCES})
target_link_libraries(connection_manager PRIVATE pqxx)

# Source files for the login repo
file(GLOB LOGIN_REPO_SOURCES "db/login/*.cpp")
add_library(login_repo STATIC ${LOGIN_REPO_SOURCES})
target_link_libraries(login_repo PRIVATE connection_manager)

# Source files for the user repo
file(GLOB USER_REPO_SOURCES "db/users/*.cpp")
add_library(user_repo STATIC ${USER_REPO_SOURCES})
target_link_libraries(user_repo PRIVATE connection_manager)


# Source files for the login service
file(GLOB LOGIN_SERVICE_SOURCES "services/login/*.cpp")
add_library(login_service STATIC ${LOGIN_SERVICE_SOURCES})
target_link_libraries(login_service PRIVATE login_repo)# Source files for the login service

# source files for user service
file(GLOB USER_SERVICE_SOURCES "services/users/*.cpp")
add_library(user_service STATIC ${USER_SERVICE_SOURCES})
target_link_libraries(user_service PRIVATE user_repo app_helper)



# Source files for the main executable
add_executable(terminet main.cpp)
target_link_libraries(
  terminet
  PRIVATE
  pqxx
  login_service
  user_service
)
