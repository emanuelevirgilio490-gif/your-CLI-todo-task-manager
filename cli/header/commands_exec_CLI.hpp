/*
In this file.hpp there are all of the functions needed for
executing commands from the user in the 'shell'
*/

#ifndef COMMANDS_EXEC_CLI_HPP
#define COMMANDS_EXEC_CLI_HPP

#include "defines.hpp"
#include "manager.hpp"
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

inline Tasks::Task *GetTask(void) {
  std::cout << "DEBUG: Entered get task\n";
  Tasks::Task *task = new Tasks::Task;
  std::string type_str{};

  std::cout << "Add Task:\n";

  // title
  std::cout << "Insert the task title: ";
  std::getline(std::cin, task->title);

  // description
  std::cout << "Add a short description: ";
  std::getline(std::cin, task->description);

  // type
  bool valid = false;
  do {
    std::cout << "Add a type: ";
    std::getline(std::cin, type_str);

    // check if the type is valid
    for (auto str : Tasks::Type_to_str) {
      if (type_str == str)
        valid = true;
    }

    if (type_str.length() == 1 && type_str[0] == 'q') {
      return nullptr;
    } else if (!valid) {
      std::cout << "Type not valid, 'q' to quit\nthen you can read the "
                   "possible type typing 'help' in the cli\n";
    }
  } while (!valid);

  return task;
}

inline int PrintTasks(void) {
  // checking if the file exist
  nlohmann::ordered_json *json_tasks = Manager::ReadTasks();
  if (json_tasks == nullptr) {
    std::cerr << "Error: the file is not open.\n";
    std::cout << "Cannot read the tasks\n";
    return FILE_NOT_OPEN;
  }

  auto &task_data = *json_tasks;

  // printing the content of the file
  std::cout << "tasks:\n";
  for (auto &[task, dict] : task_data.items()) {
    std::cout << "\ttask : " << task << '\n';
    for (auto &[key, value] : dict.items()) {
      std::cout << "\t\t" << key << " : " << value << '\n';
    }
  }

  delete json_tasks;
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
