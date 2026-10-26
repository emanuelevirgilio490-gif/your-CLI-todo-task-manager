#ifndef CLI_HPP
#define CLI_HPP

#include "defines.hpp"
#include "tasks.hpp"
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <nlohmann/json.hpp>

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

class Product_Manager {
public:
  Product_Manager(void) {}
  int AddTask(const Tasks::Task &t) {
    std::ofstream file(FILE_FULL_PATH);

    if (!file.is_open()) {
      return -1;
    }

    // set the t.type enum -> str to write it on a json
    const std::string type_str = t.type == Tasks::Type::URGENT     ? "urgent"
                                 : t.type == Tasks::Type::NORMAL   ? "normal"
                                 : t.type == Tasks::Type::OPTIONAL ? "optional"
                                 : t.type == Tasks::Type::NOT_DEFINED
                                     ? "not_defined"
                                     : "not_defined";

    // json data
    nlohmann::json task_json;
    task_json["title"] = t.title;
    task_json["description"] = t.description;
    task_json["type"] = type_str;

    // write json data to the file
    file << task_json.dump(4);

    return 0;
  }
};
}; // namespace Manager

#endif
