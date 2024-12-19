cmake_minimum_required(VERSION 3.10)
project(terminet)

set(CMAKE_CXX_STANDARD 20)

# Find Pqxx
find_package(PostgreSQL REQUIRED)
if(NOT PostgreSQL_FOUND)
    message(FATAL_ERROR "PostgreSQL development files are required to compile this project")
endif()

# Source files for the login repo
file(GLOB LOGIN_REPO_SOURCES "db/login/*.cpp")
file(GLOB APP_HELPER_SOURCES "helpers/*.cpp")

add_library(login_repo STATIC ${LOGIN_REPO_SOURCES})
add_library(app_helper STATIC ${APP_HELPER_SOURCES})
target_link_libraries(login_repo PRIVATE pqxx)

# Source files for the main executable
add_executable(terminet main.cpp)
target_link_libraries(terminet pqxx login_repo app_helper)
