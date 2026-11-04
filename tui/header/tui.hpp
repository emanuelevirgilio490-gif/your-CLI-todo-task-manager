#ifndef TUI_HPP
#define TUI_HPP

#include "command-exec-tui.hpp"
#include "manager.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/deprecated.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/ftxui.hpp>
#include <ftxui/screen/screen.hpp>

#define DEBUG_MODE

#ifdef DEBUG_MODE
inline std::ofstream log_file("log.txt");
#endif

enum class TUI_State { PRINT_HELP, NOTHING };

namespace TUIInterface {
class TUI {
public:
  void ExecTUI(void) { GetCommand(); }
  bool ShouldExit(void) { return exit; }

  ~TUI(void) {}

private:
  std::string current_command{};
  Manager::ValidCommands current_enum;

  TUI_State state = TUI_State::NOTHING;
  bool exit{};

  // TUI functions
  void GetCommand(void) {
    std::string &last_command = current_command;
    auto screen = ftxui::ScreenInteractive::TerminalOutput();

    ftxui::InputOption input_option;
    input_option.multiline = false;

    ftxui::Component command_input =
        ftxui::Input(&current_command, "command here", input_option);

    ftxui::Element tasks_window =
        ftxui::flex(ftxui::vbox(ftxui::text("hello world")));

    ftxui::Component exit_component =
        ftxui::CatchEvent(command_input, [&](ftxui::Event event) {
          // check if it should exit
          if (ShouldExit()) {
            screen.Exit();
          }

          // shortcuts check
          if (event == ftxui::Event::CtrlQ) {
            exit = true;
            return true;
          }

          // command check

          // // if the last command doesen't change
          // // we exit the function
          // if (last_command == current_command) {
          //   return false;
          // }

          if (event == ftxui::Event::Return) {
            // check for the new command
            ConvertCCToLower();
            ConvertCommand();
            ExecCommand();
            current_command = "";
            return true;
          }

          return false;
        });

    auto renderer = ftxui::Renderer(exit_component, [&] {
      return ftxui::vbox(
          {ftxui::hbox({ftxui::text("> "),
                        command_input->Render() |
                            ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 50),
                        ftxui::separator(), tasks_window,

                        // check if the TUI needs to print help
                        state == TUI_State::PRINT_HELP
                            ? TUIExecCommand::PrintHelp()
                            : ftxui::text("")}) |
           ftxui::border});
    });

    screen.Loop(renderer);
  }

  void ConvertCCToLower(void) {
    // convert the command to lower
    std::transform(current_command.begin(), current_command.end(),
                   current_command.begin(), [](char c) { return tolower(c); });
  }

  void ConvertCommand(void) {
    // convert command
    if (current_command == "quit") {
      current_enum = Manager::ValidCommands::QUIT;
    } else if (current_command == "help") {
      current_enum = Manager::ValidCommands::HELP;
    }
  }

  void ExecCommand(void) {
    switch (current_enum) {
    case Manager::ValidCommands::QUIT:
      exit = true;
      break;

    case Manager::ValidCommands::HELP:
      state = TUI_State::PRINT_HELP;
      break;

    case Manager::ValidCommands::SHOW_TASK:
      break;

    default:
      state = TUI_State::NOTHING;
    }
  }
};
}; // namespace TUIInterface

#endif
