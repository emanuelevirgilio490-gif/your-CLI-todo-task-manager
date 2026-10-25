#ifndef CLI_HPP
#define CLI_HPP

#include "commands_exec.hpp"
#include <iostream>

namespace CliInterface {
enum class ValidCommands { HELP = 0, QUIT, NOT_DEFINED };

class CLI {
  std::string last_command{};
  bool exit;
  ValidCommands last_command_enum;

public:
  CLI(void) { last_command_enum = ValidCommands::NOT_DEFINED; }

  // getters
  std::string GetCommand(void) {
    std::cout << "> ";
    getline(std::cin, last_command);
    return last_command;
  }
  std::string GetLastCommand(void) { return last_command; }
  ValidCommands GetCommandEnum(void) { return last_command_enum; }

  // ----------------- //

  ValidCommands ConvertCommand(std::string command) {
    if (command == "quit") {
      last_command_enum = ValidCommands::QUIT;
    }

    return last_command_enum;
  }
  int ExecCommand(ValidCommands c_enum) {
    switch (c_enum) {
    case ValidCommands::QUIT:
      exit = true;
      break;
    case ValidCommands::HELP:
      CommandExecution::PrintHelp();
      break;
    case ValidCommands::NOT_DEFINED:
      return 1;
      break;
    }
    return 0;
  }

  bool ShouldExit(void) { return exit; }
};
}; // namespace CliInterface

#endif
