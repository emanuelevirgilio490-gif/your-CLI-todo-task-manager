#ifndef MANAGER_H
#define MANAGER_H

#include "defines.hpp"
#include "tasks.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

namespace Manager {
enum class ValidCommands {
  HELP = 0,
  QUIT,
  ADD_TASK,
  SHOW_TASK,
  CLEAR,
  TODAY,
  DELETE_TASKS,
  NOT_DEFINED,
  NOTHING
};

inline char *commands_str[] = {(char *)"help",         (char *)"quit",
                               (char *)"add task",     (char *)"show tasks",
                               (char *)"clear",        (char *)"today",
                               (char *)"delete tasks", nullptr};

inline int AddTask(const Tasks::Task *t) {
  std::ifstream file_in(FILE_FULL_PATH);

  if (!file_in.is_open()) {
    return FILE_NOT_OPEN;
  }

  bool file_empty = false;
  if (file_in.peek() == std::fstream::traits_type::eof()) {
    file_empty = true;
  }

  int tasks_num = 0;
  std::ofstream file_out(FILE_FULL_PATH);
  if (!file_out.is_open()) {
    return FILE_NOT_OPEN;
  }

  if (!file_empty) {
    std::string content, line;
    while (std::getline(file_in, line)) {
      content += line + '\n';
    }

    nlohmann::ordered_json json_content = nlohmann::json::parse(content);

    for (auto &[key, value] : json_content.items()) {
      tasks_num++;
    }

    // set the t.type enum -> str to write it on a json
    const std::string type_str = t->type == Tasks::Type::URGENT     ? "urgent"
                                 : t->type == Tasks::Type::NORMAL   ? "normal"
                                 : t->type == Tasks::Type::OPTIONAL ? "optional"
                                 : t->type == Tasks::Type::NOT_DEFINED
                                     ? "not_defined"
                                     : "not_defined";

    // json data
    json_content["task_" + std::to_string(tasks_num + 1)] =
        nlohmann::ordered_json{{"title", t->title},
                               {"description", t->description},
                               {"type", type_str}};
    // write json data to the file
    file_out << json_content.dump(4);
  } else {
    nlohmann::ordered_json json_task;

    // set the t.type enum -> str to write it on a json
    const std::string type_str = t->type == Tasks::Type::URGENT     ? "urgent"
                                 : t->type == Tasks::Type::NORMAL   ? "normal"
                                 : t->type == Tasks::Type::OPTIONAL ? "optional"
                                 : t->type == Tasks::Type::NOT_DEFINED
                                     ? "not_defined"
                                     : "not_defined";

    // json data
    json_task["task_" + std::to_string(tasks_num + 1)] =
        nlohmann::ordered_json{{"title", t->title},
                               {"description", t->description},
                               {"type", type_str}};

    // write json data to the file
    file_out << json_task.dump(4);
  }

  file_in.close();
  file_out.close();

  return 0;
} // namespace Manager

inline nlohmann::ordered_json *ReadTasks(void) {
  // checking if the file exist
  std::ifstream file(FILE_FULL_PATH);
  if (!file.is_open()) {
    return nullptr;
  }

  // check if the file is empty
  if (file.peek() == std::fstream::traits_type::eof()) {
    return nullptr;
  }

  // reading the file
  nlohmann::ordered_json *data_tasks = new nlohmann::ordered_json;
  *data_tasks = nlohmann::ordered_json::parse(file);

  return data_tasks;
}

inline int ClearFile(void) {
  // trunc = clear the file
  std::fstream file(FILE_FULL_PATH, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    return FILE_NOT_OPEN;
  }
  file.close();

  return EXIT_SUCCESS;
}

}; // namespace Manager

#endif
