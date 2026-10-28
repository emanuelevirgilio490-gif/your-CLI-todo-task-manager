/*
This file.hpp defines types of tasks
# enums
# struct
to organize tasks
*/

#ifndef TASK_HPP
#define TASK_HPP

#include <array>
#include <string>

namespace Tasks {
enum class Type { URGENT = 0, NORMAL, OPTIONAL, NOT_DEFINED };
inline std::array<std::string, 10> type_to_str{
    {"urgent", "normal", "optional", "not defined"}};

struct Task {
  std::string description{};
  std::string title{};
  Type type;
};

inline std::string TypeToStr(const Type t) {
  switch (t) {
  case Type::URGENT:
    return "urgent";
  case Type::NORMAL:
    return "normal";
  case Type::OPTIONAL:
    return "optional";
  // not defined ecc...
  default:
    return "not defined";
  }
}
}; // namespace Tasks

#endif
