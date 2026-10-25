#include "cli.hpp"
#include "commands_exec.hpp"
#include "tasks.hpp"
#include <iostream>

#define DEBUG_MODE

int main(void) {
  CliInterface::CLI cli;

  while (cli.ShouldExit()) {
    std::string command_str = cli.GetCommand();
    CliInterface::ValidCommands command_enum = cli.ConvertCommand(command_str);

#ifdef DEBUG_MODE
    std::cout << "DEBUG: Last command = " << cli.GetLastCommand() << '\n';
#endif

    int exit_code = cli.ExecCommand(command_enum);
    if (exit_code != 0) {
      // check different exit code
      std::cout << "Error: command not valid\n";
    }
  }

  CommandExecution::Quit();

  return 0;
}
