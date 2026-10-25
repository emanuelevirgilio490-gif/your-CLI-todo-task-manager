cmake_minimum_required(VERSION 3.28)
project(todo_task_manager LANGUAGES CXX)

add_executable(todo_task_manager src/main.cpp)
target_include_directories(todo_task_manager PRIVATE "${CMAKE_SOURCE_DIR}/headers")
