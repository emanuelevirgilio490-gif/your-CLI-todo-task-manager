#ifndef COMMAND_EXEC_TUI_HPP
#define COMMAND_EXEC_TUI_HPP

#include "manager.hpp"
#include "tasks.hpp"
#include <array>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/ftxui.hpp>
#include <ftxui/screen/color.hpp>
#include <nlohmann/json_fwd.hpp>
#include <vector>

namespace TUIExecCommand {
inline ftxui::Element PrintHelp(void) {
  return ftxui::window(ftxui::text("OUTPUT"),
                       ftxui::hbox({ftxui::text("Hello World")}));
}

inline ftxui::Element ShowTasks(void) {
  nlohmann::ordered_json *json_data = Manager::ReadTasks();
  ftxui::Element window_error = ftxui::window(
      ftxui::text("Tasks"), {ftxui::text("Cannot read the tasks...") |
                             ftxui::color(ftxui::Color::Red) | ftxui::bold});

  if (json_data == nullptr) {
    return window_error;
  }

  std::vector<ftxui::Element> elements = {};
  elements.push_back(ftxui::text("Tasks:"));

  int t_index = 1;
  for (auto &[key, value] : json_data->items()) {
    std::string current_title = value["title"];
    elements.push_back(
        ftxui::text("· T" + std::to_string(t_index) + " : " + current_title));

    t_index++;
  }

  delete json_data;
  return ftxui::flex(ftxui::vbox(elements));
}

// inline ftxui::Element AddTask(void) {
//   Tasks::Task *t = new Tasks::Task;
//   Manager::AddTask(t);
// }
}; // namespace TUIExecCommand

#endif
