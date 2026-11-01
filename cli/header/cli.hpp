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
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <optional>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>

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
  // control flow
  std::string last_command{};
  bool exit;
  Manager::ValidCommands last_command_enum;

  // config
  std::string title;
  std::string PS1_text{};
  CliColors::Colors_enum PS1_color = CliColors::Colors_enum::WHITE;

  std::string prompt;

public:
  CLI(void) {
    last_command_enum = Manager::ValidCommands::NOT_DEFINED;

    CliConfig::Config cli_config;
    bool need_text = false, need_color = false;

    // check if there was an error in opening the state/config
    if (cli_config.isStateNULL() || cli_config.IsConfigOpen()) {
      std::cerr
          << "Some errors were found while opening the lua state/config\n";
      cli_config.LoadStdConfig(PS1_text, PS1_color);
      return;
    }

    // search for the PS1
    std::cout << "Searching for the PS1...\n";
    int PS1_found = cli_config.SearchPS1();

    // check if the table is nil
    if (PS1_found == PS1_IS_NIL) {
      std::cerr << "PS1 is nil. Loading the std config\n";
      cli_config.LoadStdConfig(PS1_text, PS1_color);
      return;
    }

    // if no value where found, load the std config
    if (PS1_found & NO_PS1_COLOR && PS1_found & NO_PS1_TEXT) {
      std::cerr << "No PS1 color and text found\n";
      cli_config.LoadStdConfig(PS1_text, PS1_color);
      return;
    }

    std::optional<std::array<std::string, 2>> PS1 = cli_config.GetPS1();

    if (!PS1.has_value()) {
      std::cerr << "Cannot get data from the PS1\n";
      cli_config.LoadStdConfig(PS1_text, PS1_color);
      return;
    }

    PS1_text =
        PS1_found & NO_PS1_TEXT ? CliConfig::std_config.at("text") : (*PS1)[0];
    PS1_color = PS1_found & NO_PS1_COLOR
                    ? CliColors::StrToColor(CliConfig::std_config.at("color"))
                    : CliColors::StrToColor((*PS1)[1]);

    // get the title
    title = cli_config.GetTitle();
    if (title.empty()) {
      std::cerr << "Title is empty. Loading the std title\n";
      cli_config.LoadStdTitle(title);
    }

    prompt = CliColors::escape_codes.at(PS1_color) + PS1_text +
             CliColors::escape_codes.at(CliColors::Colors_enum::RESET);
  }

  // getters
  void GetCommand(void) {
    char *input = AutoComplete::GetRawInput(prompt.c_str());
#ifdef DEBUG_MODE
    std::cout << "raw input = " << input << '\n';
#endif

    last_command = std::string(input);

#ifdef DEBUG_MODE
    std::cout << "last_command = " << last_command << '\n';
#endif

    add_history(input);
    free(input);
  }
  const std::string &GetLastCommand(void) { return last_command; }
  const Manager::ValidCommands &GetCommandEnum(void) {
    return last_command_enum;
  }

  // ----------------- //

  void PrintTitle(void) { puts(title.c_str()); }

  void ConvertToLower() {
    std::transform(last_command.begin(), last_command.end(),
                   last_command.begin(),
                   [](uint c) { return std::tolower(c); });
  }

  Manager::ValidCommands ConvertCommand(void) {
    if (last_command == "quit") {
      last_command_enum = Manager::ValidCommands::QUIT;
    } else if (last_command == "help") {
      last_command_enum = Manager::ValidCommands::HELP;
    } else if (last_command == "add task") {
      last_command_enum = Manager::ValidCommands::ADD_TASK;
    } else if (last_command == "show tasks") {
      last_command_enum = Manager::ValidCommands::SHOW_TASK;
    } else if (last_command == "clear") {
      last_command_enum = Manager::ValidCommands::CLEAR;
    } else if (last_command == "today") {
      last_command_enum = Manager::ValidCommands::TODAY;
    } else if (last_command.empty()) {
      last_command_enum = Manager::ValidCommands::NOTHING;
    } else if (last_command == "delete tasks") {
      last_command_enum = Manager::ValidCommands::DELETE_TASKS;
    }

    return last_command_enum;
  }

  int ExecCommand(void) {
    int command_success = 0;
    switch (last_command_enum) {
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
