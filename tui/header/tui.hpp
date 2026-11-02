#ifndef TUI_HPP
#define TUI_HPP

#include "manager.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/deprecated.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/ftxui.hpp>
#include <ftxui/screen/screen.hpp>

namespace TUIInterface {

class TUI {
public:
  void GetCommand(void) {
    // get the command
    ftxui::Component command_input =
        ftxui::Input(&current_command, "write your command");
    ftxui::Element tasks_window =
        ftxui::flex(ftxui::vbox(ftxui::text("hello world")));

    auto renderer = ftxui::Renderer(command_input, [&] {
      return ftxui::hbox({ftxui::text("> "), command_input->Render(),
                          ftxui::separator(), tasks_window});
    });

    auto screen = ftxui::ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);
  }

  void ConvertCommand(void) {
    // convert command
  }

  void ExecCommand(void) {
    // exec the command
  }

  void RenderScreen(void) {
    // render the things to the screen
  }

  void ClearScreen(void) {
    auto screen = ftxui::ScreenInteractive::Fullscreen();
    screen.Clear();
  }

  bool ShouldExit(void) { return exit; }

private:
  std::string current_command{};
  Manager::ValidCommands current_enum;
  bool exit;
};
} // namespace TUIInterface

#endif
