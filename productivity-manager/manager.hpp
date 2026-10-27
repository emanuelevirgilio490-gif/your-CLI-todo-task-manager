#ifndef MANAGER_H
#define MANAGER_H

#include "defines.hpp"
#include "tasks.hpp"
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
  NOT_DEFINED
};

inline int AddTask(const Tasks::Task *t) {
  std::ofstream file(FILE_FULL_PATH);

  if (!file.is_open()) {
    return FILE_NOT_OPEN;
  }

  // set the t.type enum -> str to write it on a json
  const std::string type_str = t->type == Tasks::Type::URGENT     ? "urgent"
                               : t->type == Tasks::Type::NORMAL   ? "normal"
                               : t->type == Tasks::Type::OPTIONAL ? "optional"
                               : t->type == Tasks::Type::NOT_DEFINED
                                   ? "not_defined"
                                   : "not_defined";

  // json data
  nlohmann::ordered_json task_json;
  task_json["task"] = nlohmann::ordered_json{
      {"title", t->title}, {"description", t->description}, {"type", type_str}};

  // write json data to the file
  file << task_json.dump(4);

  return 0;
} // namespace Manager

inline nlohmann::ordered_json *ReadTasks(void) {
  // checking if the file exist
  std::ifstream file(FILE_FULL_PATH);
  if (!file.is_open()) {
    return nullptr;
  }

  // reading the file
  nlohmann::ordered_json *data_tasks = new nlohmann::ordered_json;
  *data_tasks = nlohmann::ordered_json::parse(file);

  return data_tasks;
}

}; // namespace Manager

#endif
