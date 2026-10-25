#ifndef CLI_HPP
#define CLI_HPP

#include "commands_exec.hpp"
#include "defines.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>

namespace CliInterface {
enum class ValidCommands { HELP = 0, QUIT, ADD_TASK, SHOW_TASK, NOT_DEFINED };

class CLI {
  std::string last_command{};
  bool exit;
  ValidCommands last_command_enum;

public:
  CLI(void) { last_command_enum = ValidCommands::NOT_DEFINED; }

  // getters
  std::string GetCommand(void) {
    std::cout << "> ";
    std::getline(std::cin, last_command);

    return last_command;
  }
  std::string &GetLastCommand(void) { return last_command; }
  ValidCommands GetCommandEnum(void) { return last_command_enum; }

  // ----------------- //

  void ConvertToLower(std::string &s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](uint c) { return std::tolower(c); });
  }

  ValidCommands ConvertCommand(std::string command) {
    if (command == "quit") {
      last_command_enum = ValidCommands::QUIT;
    } else if (command == "help") {
      last_command_enum = ValidCommands::HELP;
    } else if (command == "add task") {
      last_command_enum = ValidCommands::ADD_TASK;
    } else if (command == "show tasks") {
      last_command_enum = ValidCommands::SHOW_TASK;
    }
    return last_command_enum;
  }

  int ExecCommand(ValidCommands c_enum) {
    int command_success = 0;
    switch (c_enum) {
    case ValidCommands::QUIT:
      exit = true;
      std::cout << "Exit...\n";
      break;
    case ValidCommands::HELP:
      CommandExecution::PrintHelp();
      break;
    case ValidCommands::NOT_DEFINED:
      command_success = ENUM_NOT_DEFINED;
      std::cout << "NOT DEFINED\n";
      break;
    case ValidCommands::ADD_TASK:
      std::cout << "Adding task...";
      break;
    case ValidCommands::SHOW_TASK:
      std::cout << "Showing tasks...\n";
      command_success = CommandExecution::PrintTasks();
      break;
    default:
      std::cout << "the command is not handled\n";
      break;
    }
    return command_success;
  }

  bool ShouldExit(void) { return exit; }

  void Reset(void) {
    last_command = "";
    last_command_enum = ValidCommands::NOT_DEFINED;
  }
};
}; // namespace CliInterface

#endif
