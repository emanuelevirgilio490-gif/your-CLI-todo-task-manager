#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/ftxui.hpp>

int main(void) {
  auto screen = ftxui::ScreenInteractive::TerminalOutput();

  std::string i1, i2, i3;
  auto input1 = ftxui::Input(&i1, "Input1");
  auto input2 = ftxui::Input(&i2, "Input2");
  auto input3 = ftxui::Input(&i3, "Input3");

  auto Inputs = ftxui::Container::Vertical({input1, input2, input3});
  auto renderer = ftxui::Renderer(Inputs, [&]() {
    return ftxui::vbox(
        {ftxui::vbox({input1->Render(), input2->Render(), input3->Render()}),

         ftxui::separator(),

         ftxui::vbox({ftxui::text(i1), ftxui::text(i2), ftxui::text(i3)})});
  });

  screen.Loop(renderer);
  return 0;
}
