#include <ftxui/component/app.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/ftxui.hpp>
#include <ftxui/screen/color.hpp>

int main(void) {
  ftxui::Element element =
      ftxui::vbox(ftxui::text("FTXUI HI!") | ftxui::color(ftxui::Color::Red));

  auto screen = ftxui::Screen::Create(ftxui::Dimension::Full(),
                                      ftxui::Dimension::Fit(element));
  Render(screen, element);
  screen.Print();
  return 0;
}
