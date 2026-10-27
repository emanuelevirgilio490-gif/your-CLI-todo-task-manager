cmake_minimum_required(VERSION 3.16)

project(productivity-cli LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DEBUG_MODE ON)
if(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(cli
    cli/src/cli.cpp
)

target_include_directories(cli PRIVATE
    cli/header
    productivity-manager
)
