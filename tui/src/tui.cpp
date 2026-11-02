#include "tui.hpp"
#include <iostream>

int main(void) {
  // clears the screen
  std::cout << "\e[2J\e[H";

  TUIInterface::TUI tui;

  while (!tui.ShouldExit()) {
    // do something
    tui.ClearScreen();
    tui.GetCommand();
  }

  return 0;
}
