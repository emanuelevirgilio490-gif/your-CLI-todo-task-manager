#ifndef COMMANDS_EXEC_HPP
#define COMMANDS_EXEC_HPP

#include <iostream>

namespace CommandExecution {
inline void PrintHelp(void) {
  std::cout << "Hi! Welcome to the CLI todo task manager:\nhere you can start "
               "planning what you wanna do today!\n";
}

inline void Quit(void) {
  std::cout << "Bye! Have a nice day!\n";
  exit(0);
}
}; // namespace CommandExecution

#endif
