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
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>

#define DEBUG_MODE

#ifdef DEBUG_MODE
inline std::ofstream log_file("log.txt");
#endif

enum class TUI_State { PRINT_HELP, NOTHING, INVALID_COMMAND };

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
        ftxui::flex(ftxui::vbox(ftxui::text("hello world") | ftxui::italic));

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

            // check if the user need to exit
            // (after typing a command)
            if (ShouldExit()) {
              screen.Exit();
              return true;
            }

            // clear the last command
            current_command = "";
            return true;
          }

          return false;
        });

    auto renderer = ftxui::Renderer(exit_component, [&] {
      return ftxui::vbox(
          {// TITLE
           ftxui::text("Productivity - TUI") | ftxui::center | ftxui::bold |
               ftxui::color(ftxui::Color::Cyan),

           // Command line
           ftxui::hbox({
               ftxui::text("> "),
               command_input->Render() |
                   ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 50) |
                   ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1),

               ftxui::separator() | ftxui::color(ftxui::Color::DarkCyan),

               // TASKS
               tasks_window,
           }) | ftxui::borderStyled(ftxui::Color::DarkCyan),

           // Output
           state == TUI_State::INVALID_COMMAND
               ? ftxui::text("ERROR: Command not valid") |
                     ftxui::color(ftxui::Color::Red1) | ftxui::bold
               : ftxui::text(""),

           // check if the TUI needs to print help
           state == TUI_State::PRINT_HELP
               ? TUIExecCommand::PrintHelp() | ftxui::align_right
               : ftxui::text("")});
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
    } else {
      current_enum = Manager::ValidCommands::NOT_DEFINED;
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

    case Manager::ValidCommands::NOT_DEFINED:
      state = TUI_State::INVALID_COMMAND;
      break;

    default:
      state = TUI_State::NOTHING;
    }
  }
};
}; // namespace TUIInterface

#endif
