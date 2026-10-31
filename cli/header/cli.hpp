/*
This file.hpp is for the CLI interface
It's like a shell

what does it do:
# get the command from the user
# convert it
# calls other functions to execute those commands
# return the exit codes (defined in defines.hpp)
*/

#ifndef CLI_HPP
#define CLI_HPP

#include "cli-colors.hpp"
#include "cli-config.hpp"
#include "commands_exec_CLI.hpp"
#include "defines.hpp"
#include "manager.hpp"
#include "tasks.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>

namespace CliInterface {
namespace AutoComplete {
inline char *GetRawInput(const char *prompt) {
  char *raw_input = readline(prompt);
  return raw_input;
}

// Funzione che viene chiamata quando premiamo Tab
inline int CustomComplete(int count, int key) {
  // Prendiamo quello che l'utente ha scritto finora sulla riga
  std::string current(rl_line_buffer);

  // Cerchiamo un comando che inizi con quello che ha scritto l'utente
  for (int i = 0; Manager::commands_str[i] != nullptr; ++i) {
    std::string cmd(Manager::commands_str[i]);
    // Se il comando inizia con il testo digitato e non è già completo
    if (cmd.rfind(current, 0) == 0 && cmd != current) {
      // Calcoliamo la parte mancante da aggiungere
      std::string missing = cmd.substr(current.length());

      // FORZIAMO L'INSERIMENTO DEL TESTO MANCANTE!
      rl_insert_text(missing.c_str());
      return 0;
    }
  }

  return 0;
}

inline void SetFunction(void) {
  // Disattivammo il vecchio sistema di completamento automatico bacato
  rl_attempted_completion_function = nullptr;

  // Associamo il tasto Tab (codice 9) alla nostra funzione personalizzata!
  rl_bind_key(9, CustomComplete);
}
}; // namespace AutoComplete

class CLI {
  std::string last_command{};
  bool exit;
  Manager::ValidCommands last_command_enum;
  std::string PS1_text;
  CliColors::Colors_enum PS1_color;

public:
  CLI(void) {
    last_command_enum = Manager::ValidCommands::NOT_DEFINED;

    CliConfig::Config cli_config;
    bool error_found = false;

    if (cli_config.IsConfigOpen() != EXIT_SUCCESS) {
      std::cerr << "ERROR: cannot open the config";
      error_found = true;
    }
    if (cli_config.isStateNULL()) {
      std::cerr << "ERROR: The lua_state is NULL";
      error_found = true;
    }
    if (cli_config.SearchPS1() != EXIT_SUCCESS) {
      std::cerr << "ERROR: there is not PS1 in the fille";
      error_found = true;
    }
    if (cli_config.GetPS1() == PS1_ERROR) {
      std::cerr << "ERROR: cannot get the PS1 text\n";
      error_found = true;
    }

    if (error_found) {
      std::cout << "INFO: loading the default config\n";
      PS1_text = CliConfig::std_config.at("text");
      PS1_color = CliColors::StrToColor(CliConfig::std_config.at("color"));
    } else {
      std::cout << "INFO: no errors found, loading the config from the file\n";
      PS1_text = cli_config.GetPS1();
    }

    std::cout << "Config ended.\n";
  }

  // getters
  std::string GetCommand(void) {
    char *input = AutoComplete::GetRawInput(PS1_text.c_str());
#ifdef DEBUG_MODE
    std::cout << "raw input = " << input << '\n';
#endif

    last_command = std::string(input);

#ifdef DEBUG_MODE
    std::cout << "last_command = " << last_command << '\n';
#endif

    add_history(input);
    free(input);

    return last_command;
  }
  std::string &GetLastCommand(void) { return last_command; }
  Manager::ValidCommands GetCommandEnum(void) { return last_command_enum; }

  // ----------------- //

  void PrintTitle(void) {
    puts("|------------------------|");
    puts("|                        |");
    puts("|\033[1;0m    PRODUCTIVITY-CLI    \033[0m|");
    puts("|                        |");
    puts("|------------------------|");
  }

  void ConvertToLower(std::string &s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](uint c) { return std::tolower(c); });
  }

  Manager::ValidCommands ConvertCommand(std::string command) {
    if (command == "quit") {
      last_command_enum = Manager::ValidCommands::QUIT;
    } else if (command == "help") {
      last_command_enum = Manager::ValidCommands::HELP;
    } else if (command == "add task") {
      last_command_enum = Manager::ValidCommands::ADD_TASK;
    } else if (command == "show tasks") {
      last_command_enum = Manager::ValidCommands::SHOW_TASK;
    } else if (command == "clear") {
      last_command_enum = Manager::ValidCommands::CLEAR;
    } else if (command == "today") {
      last_command_enum = Manager::ValidCommands::TODAY;
    } else if (command.empty()) {
      last_command_enum = Manager::ValidCommands::NOTHING;
    } else if (command == "delete tasks") {
      last_command_enum = Manager::ValidCommands::DELETE_TASKS;
    }

    return last_command_enum;
  }

  int ExecCommand(Manager::ValidCommands c_enum) {
    int command_success = 0;
    switch (c_enum) {
    case Manager::ValidCommands::QUIT:
      exit = true;
      std::cout << "Exit...\n";
      break;
    case Manager::ValidCommands::HELP:
      CommandExecution::PrintHelp();
      break;
    case Manager::ValidCommands::NOT_DEFINED:
      command_success = ENUM_NOT_DEFINED;
      break;
    case Manager::ValidCommands::ADD_TASK: {
      Tasks::Task *task = CommandExecution::GetTask();
      if (task == nullptr) {
        std::cout << "Exit...\n";
        return TYPE_NOT_VALID;
      }

      std::cout << "Adding task...\n";
      Manager::AddTask(task);

      // use the task to write it on the file

      delete task;
    } break;
    case Manager::ValidCommands::SHOW_TASK:
      std::cout << "Showing tasks...\n";
      command_success = CommandExecution::PrintTasks();
      break;
    case Manager::ValidCommands::CLEAR:
      CommandExecution::Clear();
      break;
    case Manager::ValidCommands::TODAY:
      CommandExecution::PrintToday();
      break;
    case Manager::ValidCommands::DELETE_TASKS:
      CommandExecution::ClearFile();
      break;
    case Manager::ValidCommands::NOTHING:
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
    last_command_enum = Manager::ValidCommands::NOT_DEFINED;
  }
};
}; // namespace CliInterface

#endif
