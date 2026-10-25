#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>

namespace Tasks {
enum class Type { URGENT = 0, NORMAL, OPTIONAL, NOT_DEFINED };

struct Task {
  std::string description{};
  std::string title{};
  Type type;
};
}; // namespace Tasks

#endif
