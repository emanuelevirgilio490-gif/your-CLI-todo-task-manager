#include "cli.hpp"
#include "commands_exec_CLI.hpp"
#include "manager.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>

// handler for ctrl + c
void SigIntHandler(int sig) {
  std::cout << "\nPlease do not use ctrl + c, write \"quit\" in stead\n";
}

int main(void) {
  // signal handler
  signal(SIGINT, SigIntHandler);

  // title
  CommandExecution::Clear();
  CliInterface::CLI cli;
  cli.PrintTitle();

  // set the function for the readline
  CliInterface::AutoComplete::SetFunction();

  // main loop
  while (!cli.ShouldExit()) {
    // get command
    cli.GetCommand();
#ifdef DEBUG_MODE
    std::cout << "DEBUG: Last command normal = " << cli.GetLastCommand()
              << '\n';
#endif

    // convert it to lower and to an enum
    cli.ConvertToLower();
    cli.GetLastCommand();
    cli.ConvertCommand();

#ifdef DEBUG_MODE
    std::cout << "DEBUG: Last command = " << cli.GetLastCommand() << '\n';
#endif

    // Execution of the command
    int exit_code = cli.ExecCommand();
    if (exit_code != 0) {
// TODO: check different exit code
#ifdef DEBUG_MODE
      std::cout << "exit code: " << exit_code << '\n';
#endif
    }

    // newline at the end of the command
    // if it isn't the clear command
    if (!(cli.GetLastCommand() == "clear"))
      putchar('\n');

    cli.Reset();
  }

  // prints an hello
  CommandExecution::Quit();
}
