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

# Source files for the message repo
file(GLOB MESSAGE_REPO_SOURCES "db/message/*.cpp")
add_library(message_repo STATIC ${MESSAGE_REPO_SOURCES})
target_link_libraries(message_repo PRIVATE connection_manager)

# Source files for the thread repo
file(GLOB THREAD_REPO_SOURCES "db/threads/*.cpp")
add_library(thread_repo STATIC ${THREAD_REPO_SOURCES})
target_link_libraries(thread_repo PRIVATE connection_manager)

# Source files for the Category repo
file(GLOB CATEGORY_REPO_SOURCES "db/categories/*.cpp")
add_library(category_repo STATIC ${CATEGORY_REPO_SOURCES})
target_link_libraries(category_repo PRIVATE connection_manager)

# Source files for the Community repo
file(GLOB COMMUNITY_REPO_SOURCES "db/community/*.cpp")
add_library(community_repo STATIC ${COMMUNITY_REPO_SOURCES})
target_link_libraries(community_repo PRIVATE connection_manager)

# Source files for the Search repo
file(GLOB SEARCH_REPO_SOURCES "db/search/*.cpp")
add_library(search_repo STATIC ${SEARCH_REPO_SOURCES})
target_link_libraries(search_repo PRIVATE connection_manager)



# Source files for the login service
file(GLOB LOGIN_SERVICE_SOURCES "services/login/*.cpp")
add_library(login_service STATIC ${LOGIN_SERVICE_SOURCES})
target_link_libraries(login_service PRIVATE login_repo)# Source files for the login service

# source files for user service
file(GLOB USER_SERVICE_SOURCES "services/users/*.cpp")
add_library(user_service STATIC ${USER_SERVICE_SOURCES})
target_link_libraries(user_service PRIVATE user_repo app_helper)

# source files for routing service
file(GLOB ROUTING_SERVICE_SOURCES "services/routing/*.cpp")
add_library(routing_service STATIC ${ROUTING_SERVICE_SOURCES})
target_link_libraries(routing_service PRIVATE message_repo app_helper)

# source files for thread service
file(GLOB THREAD_SERVICE_SOURCES "services/threads/*.cpp")
add_library(thread_service STATIC ${THREAD_SERVICE_SOURCES})
target_link_libraries(thread_service PRIVATE thread_repo app_helper)

# source files for category service
file(GLOB CATEGORY_SERVICE_SOURCES "services/category/*.cpp")
add_library(category_service STATIC ${CATEGORY_SERVICE_SOURCES})
target_link_libraries(category_service PRIVATE category_repo app_helper)

# source files for community service
file(GLOB COMMUNITY_SERVICE_SOURCES "services/community/*.cpp")
add_library(community_service STATIC ${COMMUNITY_SERVICE_SOURCES})
target_link_libraries(community_service PRIVATE community_repo app_helper)

# source files for community service
file(GLOB SEARCH_SERVICE_SOURCES "services/search/*.cpp")
add_library(search_service STATIC ${SEARCH_SERVICE_SOURCES})
target_link_libraries(search_service PRIVATE search_repo app_helper)


# Source files for the main executable
add_executable(terminet main.cpp)
target_link_libraries(
  terminet
  PRIVATE
  pqxx
  app_helper
  login_service
  user_service
  routing_service
  thread_service
  category_service
  community_service
  search_service
)
