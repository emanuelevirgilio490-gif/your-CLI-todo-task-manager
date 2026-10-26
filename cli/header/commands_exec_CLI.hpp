/*
In this file.hpp there are all of the functions needed for
executing commands from the user in the 'shell'
*/

#ifndef COMMANDS_EXEC_HPP
#define COMMANDS_EXEC_HPP

#include "defines.hpp"
#include "tasks.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

namespace CommandExecution {
inline void PrintHelp(void) {
  std::cout << "Hi! Welcome to the CLI todo task manager:\nhere you can start "
               "planning what you wanna do today!\n";
}

inline void Quit(void) {
  std::cout << "Bye! Have a nice day!\n";
  exit(0);
}

inline int AddTask(const Tasks::Task &task) {
  std::ifstream file(FILE_FULL_PATH);
  if (!file.is_open()) {
    std::cout << "ERROR: File: " << FILE_FULL_PATH << " Does not exist!\n";
    return FILE_DOES_NOT_EXIST;
  }

  std::cout << "File does exist\nadd task is still in developement\n";
  return EXIT_SUCCESS;
}

inline int PrintTasks(void) {
  // checking if the file exist
  std::ifstream file(FILE_FULL_PATH);
  if (!file.is_open()) {
    std::cout << "ERROR: File: " << FILE_FULL_PATH << " Does not exist!\n";
    return FILE_DOES_NOT_EXIST;
  }

  // reading the file
  nlohmann::json data_tasks = nlohmann::json::parse(file);

  // printing the content of the file
  std::cout << "tasks:\n";
  for (auto &[task_name, dict] : data_tasks.items()) {
    std::cout << "\ttask: " << task_name << '\n';
    for (auto &[key, value] : dict.items()) {
      std::cout << '\t' << key << " : " << value << '\n';
    }
    std::cout << '\n';
  }

  return EXIT_SUCCESS;
}

inline void Clear(void) { std::cout << "\e[H\e[J"; }

inline void PrintToday(void) {
  // print the current date
  auto current_date =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::cout << "today is "
            << std::put_time(std::localtime(&current_date), "%d/%m/%Y") << '\n';
}
}; // namespace CommandExecution

#endif
