#include "tui.hpp"
#include "manager.hpp"
#include <iostream>

#define CLEAR_SCREEN std::cout << "\e[2J\e[H"

int main(void) {
  CLEAR_SCREEN;

  TUIInterface::TUI tui;
  tui.ExecTUI();

  CLEAR_SCREEN;

  return 0;
}
