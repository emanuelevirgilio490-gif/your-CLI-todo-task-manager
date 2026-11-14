#ifndef TUI_HPP
#define TUI_HPP

#include <fstream>
#include <ftxui/dom/elements.hpp>
#include <vector>
#define DEBUG

#include "command-exec-tui.hpp"
#include "manager.hpp"
#include "tasks.hpp"
#include <algorithm>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/ftxui.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>

#ifdef DEBUG
inline std::ofstream file_log("log.txt");
#endif

enum class TUI_State { PRINT_HELP, ADD_TASK, NOTHING, INVALID_COMMAND };
enum class AddTaskState { NOTHING, ADD_TASK, TYPE_NOT_VALID };

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
  AddTaskState add_task_state = AddTaskState::NOTHING;
  bool exit{};

  // AddTaskView data vars
  Tasks::Task new_task{};
  std::string type_str{};

  int selected_index_type = 0;
  const std::vector<std::string> &type_task_entries = Tasks::type_to_str;

  // TUI general components
  ftxui::Component tab;
  ftxui::Component add_task_renderer;
  ftxui::Component home_renderer;
  ftxui::Component tabs_content;
  ftxui::Component app;
  ftxui::Component renderer;

  // AddTaskView components
  // Inputs for the task
  ftxui::Component Input_Title;
  ftxui::Component Input_Descr;
  ftxui::Component Input_Type;
  ftxui::Component Inputs_Task;
  ftxui::Component input_handler_task;

  // container for all of the inputs and buttons
  ftxui::Component add_task_container;

  // button
  ftxui::ButtonOption b_option_task;
  ftxui::Component button_create_task;

  // HomeView components
  ftxui::Component command_input;
  ftxui::Component input_handler;

  // HomeView Task bar
  ftxui::Element tasks_window = TUIExecCommand::ShowTasks();

  void SetupComponents(ftxui::ScreenInteractive &screen) {
    ftxui::InputOption options_add_task;

    options_add_task.multiline = false;
    options_add_task.on_enter = [&] { screen.PostEvent(ftxui::Event::Tab); };

    Input_Title = ftxui::Input(&new_task.title, "Title", options_add_task);
    Input_Descr =
        ftxui::Input(&new_task.description, "Description", options_add_task);

    Input_Type = ftxui::Menu(type_task_entries, &selected_index_type);

    // container of the inputs
    Inputs_Task =
        ftxui::Container::Vertical({Input_Title, Input_Descr, Input_Type});

    input_handler_task =
        ftxui::CatchEvent(Inputs_Task, [&](ftxui::Event event) {
          if (event == ftxui::Event::Return) {
            screen.PostEvent(ftxui::Event::Tab);
          }
          return false;
        });

    b_option_task.transform = [&](const ftxui::EntryState &state) {
      return ftxui::text(state.label) |
             ftxui::borderStyled(ftxui::Color::Cyan1);
    };

    button_create_task = ftxui::Button(
        "Add Task",
        [&](void) {
#ifdef DEBUG
          file_log << "Bottone cliccato, index per il type "
                   << selected_index_type << "\n";
#endif
          new_task.type =
              Tasks::StrToType(type_task_entries[selected_index_type]);
          int success = Manager::AddTask(new_task);
#ifdef DEBUG
          file_log << "Task added with exit code " << success << '\n';
#endif

          // reset the new_task
          new_task.type = Tasks::Type::NOT_DEFINED;
          new_task.description = "";
          new_task.title = "";

          add_task_state = AddTaskState::ADD_TASK;
        },
        b_option_task);

    add_task_container =
        ftxui::Container::Vertical({Inputs_Task, button_create_task});

    // global events
    ftxui::ComponentDecorator exit_events =
        ftxui::CatchEvent([&](ftxui::Event event) {
          if (event == ftxui::Event::CtrlQ) {
            screen.Exit();
            return true;
          }
          return false;
        });
    Manager::AddTask(new_task);

    ftxui::InputOption input_option;
    input_option.multiline = false;

    command_input =
        ftxui::Input(&current_command, "command here", input_option);

    input_handler = ftxui::CatchEvent(command_input, [&](ftxui::Event event) {
      if (ShouldExit()) {
#ifdef DEBUG
        file_log.close();
#endif
        screen.Exit();
      }

      if (event == ftxui::Event::CtrlQ) {
#ifdef DEBUG
        file_log.close();
#endif
        exit = true;
        screen.Exit();
        return true;
      }

      if (event == ftxui::Event::Return) {
        ConvertCCToLower();
        ConvertCommand();
        ExecCommand();

        if (ShouldExit()) {
#ifdef DEBUG
          file_log.close();
#endif
          screen.Exit();
          return true;
        }

        current_command = "";
        return true;
      }

      return false;
    });
  }

  // TUI functions
  void GetCommand(void) {
    auto screen = ftxui::ScreenInteractive::TerminalOutput();
    screen.TrackMouse(true);

    SetupComponents(screen);

    // selected tab
    int selected = 0;

    // tabs
    std::vector<std::string> tabs = {"TUI", "Add Task"};
    tab = ftxui::Menu(&tabs, &selected);

    // renderers (usano i componenti già creati in SetupComponents)
    add_task_renderer = AddTaskView();
    home_renderer = HomeView();

    tabs_content =
        ftxui::Container::Tab({home_renderer, add_task_renderer}, &selected);

    app = ftxui::Container::Vertical({tab, tabs_content});
    app = ftxui::CatchEvent(app, [&](ftxui::Event event) {
      if (event == ftxui::Event::CtrlQ) {
        screen.Exit();
        return true;
      }
      return false;
    });

    renderer = ftxui::Renderer(app, [&] {
      return ftxui::hbox({tabs_content->Render() | ftxui::flex,
                          tab->Render() | ftxui::border | ftxui::align_right |
                              ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 10)});
    });

    screen.Loop(renderer);
  }

  ftxui::Component AddTaskView(void) {
    return ftxui::Renderer(add_task_container, [&] {
      return ftxui::window(ftxui::text("Add task:"),
                           ftxui::vbox({
                               add_task_container->Render(),

                               // checking the state to print out
                               // if the task is added
                               add_task_state == AddTaskState::TYPE_NOT_VALID
                                   ? ftxui::text("ERROR: type not valid")
                               : add_task_state == AddTaskState::ADD_TASK
                                   ? ftxui::text("Task Added!")
                                   : ftxui::text("")

                               // button_create_task->Render()
                           }));
    });
  }

  ftxui::Component HomeView(void) {
    return ftxui::Renderer(input_handler, [&] {
      tasks_window = TUIExecCommand::ShowTasks();

      // reset the add task state
      add_task_state = AddTaskState::NOTHING;
      return ftxui::vbox({
          // TITLE
          ftxui::text("Productivity - TUI") | ftxui::center | ftxui::bold |
              ftxui::color(ftxui::Color::Cyan),

          // Command line
          ftxui::hbox({
              ftxui::text("> "),
              command_input->Render() |
                  ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1),
          }) | ftxui::borderStyled(ftxui::Color::DarkCyan),

          ftxui::hbox(
              // Output
              state == TUI_State::INVALID_COMMAND
                  ? ftxui::text("ERROR: Command not valid") |
                        ftxui::color(ftxui::Color::Red1) | ftxui::bold
                  : ftxui::text(""),

              // check if the TUI needs to print help
              state == TUI_State::PRINT_HELP ? TUIExecCommand::PrintHelp()
                                             : ftxui::text(""),

              state == TUI_State::NOTHING
                  ? ftxui::text("Hey there :3") | ftxui::bold
                  : ftxui::text(""),

              ftxui::filler(),

              // TASKS
              tasks_window | ftxui::align_right | ftxui::border),
      });
    });
  }

  void ConvertCCToLower(void) {
    std::transform(current_command.begin(), current_command.end(),
                   current_command.begin(), [](char c) { return tolower(c); });
  }

  void ConvertCommand(void) {
    if (current_command == "quit") {
      current_enum = Manager::ValidCommands::QUIT;
    } else if (current_command == "help") {
      current_enum = Manager::ValidCommands::HELP;
    } else if (current_command == "add task") {
      current_enum = Manager::ValidCommands::ADD_TASK;
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

    case Manager::ValidCommands::ADD_TASK:
      state = TUI_State::ADD_TASK;
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
