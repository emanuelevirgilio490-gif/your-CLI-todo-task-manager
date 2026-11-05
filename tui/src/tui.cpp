#include "tui.hpp"
#include "manager.hpp"
#include <iostream>

#define CLEAR_SCREEN std::cout << "\e[H\e[2J"

int main(void) {
  CLEAR_SCREEN;

  TUIInterface::TUI tui;
  tui.ExecTUI();

  CLEAR_SCREEN;

  return 0;
}
