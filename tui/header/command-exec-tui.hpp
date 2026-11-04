#ifndef COMMAND_EXEC_TUI_HPP
#define COMMAND_EXEC_TUI_HPP

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/ftxui.hpp>

namespace TUIExecCommand {
inline ftxui::Element PrintHelp(void) {
  return ftxui::window(ftxui::text("OUTPUT"),
                       ftxui::hbox({ftxui::text("Hello World")}));
}
}; // namespace TUIExecCommand

#endif
