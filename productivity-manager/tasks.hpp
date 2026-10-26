/*
This file.hpp defines types of tasks
# enums
# struct
to organize tasks
*/

#ifndef TASK_HPP
#define TASK_HPP

#include <string>

namespace Tasks {
enum class Type { URGENT = 0, NORMAL, OPTIONAL, NOT_DEFINED };

struct Task {
  std::string description{};
  std::string title{};
  Type type;
};
}; // namespace Tasks

#endif
