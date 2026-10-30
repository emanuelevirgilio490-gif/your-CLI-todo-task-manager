cmake_minimum_required(VERSION 3.16)

project(cli LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DEBUG_MODE ON)
if(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# find readline
find_library(READLINE_LIBRARY NAMES readline)
find_path(READLINE_INCLUDE_DIR NAMES readline/readline.h)

# find lua_lib
find_package(Lua REQUIRED)

add_executable(cli
    cli/src/cli.cpp
)

target_include_directories(cli PRIVATE
    cli/header
    productivity-manager
)

# readline link
target_link_libraries(cli PRIVATE ${READLINE_LIBRARY})
target_include_directories(cli PRIVATE ${READLINE_INCLUDE_DIR})
# lua_lib link
target_include_directories(cli PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(cli PRIVATE ${LUA_LIBRARIES})
