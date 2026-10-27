#include "cli.hpp"
#include "commands_exec_CLI.hpp"
#include "manager.hpp"
#include "tasks.hpp"
#include <csignal>
#include <iostream>
// #define DEBUG_MODE

// handler for ctrl + c
void SigIntHandler(int sig) {
  std::cout << "Please do not use ctrl + c, write \"quit\" in stead\n";
}

int main(void) {
  // signal handler
  signal(SIGINT, SigIntHandler);

  CliInterface::CLI cli;

  while (!cli.ShouldExit()) {
    // get command
    std::string command_str = cli.GetCommand();
#ifdef DEBUG_MODE
    std::cout << "DEBUG: Last command normal = " << cli.GetLastCommand()
              << '\n';
#endif

    // convert it to lower and to an enum
    cli.ConvertToLower(cli.GetLastCommand());
    command_str = cli.GetLastCommand();
    CliInterface::ValidCommands command_enum = cli.ConvertCommand(command_str);

#ifdef DEBUG_MODE
    std::cout << "DEBUG: Last command = " << cli.GetLastCommand() << '\n';
#endif

    // Execution of the command
    int exit_code = cli.ExecCommand(command_enum);
    if (exit_code != 0) {
      // check different exit code
      std::cout << "Error: some errors occured\n";
    }

    cli.Reset();
  }

  // prints an hello
  CommandExecution::Quit();
}
