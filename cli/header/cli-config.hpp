#ifndef CLI_CONFIG_HPP
#define CLI_CONFIG_HPP

#include "cli-colors.hpp"
#include "defines.hpp"
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <string>
#include <sys/types.h>
#include <unordered_map>

extern "C" {
#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
}

namespace CliConfig {

const std::unordered_map<std::string, std::string> std_config = {
    {"text", "> "},
    {"color", "white"},
    {"title", "|------------------------|\n"
              "|                        |\n|\033[1;0m    PRODUCTIVITY-CLI    "
              "\033[0m|\n|     "
              "                   |\n|------------------------|"}};

class Config {
public:
  Config(void) {
    L = luaL_newstate();
    luaL_openlibs(L);
  }
  bool isStateNULL(void) { return L == NULL; }

  int IsConfigOpen(void) {
#ifdef DEBUG_MODE
    std::cout << "Checking if the config is open";
#endif
    // check
    if (luaL_dofile(L, CONFIG_FULL_PATH.c_str()) != LUA_OK) {
      std::cout << "Error: " << lua_tostring(L, -1);
      lua_pop(L, 1);
      return CONFIG_NOT_OPENED;
    }

    luaL_openlibs(L);
    return EXIT_SUCCESS;
  }

  int SearchPS1(void) {
#ifdef DEBUG_MODE
    std::cout << "Checking there is a PS1 in the config\n";
#endif
    int state = 0;
    lua_getglobal(L, "PS1");
    // check if PS1 is nil
    if (lua_isnil(L, -1)) {
      lua_pop(L, 1);
      return PS1_IS_NIL;
    }
    lua_getfield(L, -1, "text");

    // check if the PS1 text is a string
    if (!lua_isstring(L, -1)) {
      std::cerr << "Error: PS1 text is not a string\n";
      state |= NO_PS1_TEXT;
    }

    // check if the PS1 color is a string
    // PS1 is now in the -2 index
    lua_getfield(L, -2, "color");
    if (!lua_isstring(L, -1)) {
      std::cerr << "Error: PS1 color is not a string\n";
      state |= NO_PS1_COLOR;
    }

    lua_pop(L, 3);

    return state;
  }

  // call this func after the SearchPS1()
  std::optional<std::array<std::string, 2>> GetPS1(void) {
#ifdef DEBUG_MODE
    std::cout << "Getting the PS1...\n";
#endif

    std::array<std::string, 2> PS1;
    PS1[0] = "";
    PS1[1] = "";

    std::string text{}, color{};

    // check if it is a table
    lua_getglobal(L, "PS1");
    if (!lua_istable(L, -1)) {
      lua_pop(L, 1);
      return std::nullopt;
    }

    // get PS1 table
    if (lua_istable(L, -1)) {
      // get the PS1 text
      lua_getfield(L, -1, "text");

      if (lua_isstring(L, -1)) {
        text = lua_tostring(L, -1);
      }
      lua_pop(L, 1);

      // get the PS1 color
      lua_getfield(L, -1, "color");

      if (lua_isstring(L, -1)) {
        color = lua_tostring(L, -1);
      }
      lua_pop(L, 1); // color pop

      lua_pop(L, 1); // table pop
    }

    PS1[0] = text;
    PS1[1] = color;

    return PS1;
  }

  void LoadStdConfig(std::string &ps1_t, CliColors::Colors_enum &ps1_c) {
    ps1_t = std_config.at("text");
    ps1_c = CliColors::StrToColor(std_config.at("color"));
  }

  void LoadStdTitle(std::string &title) { title = std_config.at("title"); }

  std::string GetTitle(void) {
    std::string title{};
    lua_getglobal(L, "TITLE");
    if (lua_isstring(L, -1)) {
      title = lua_tostring(L, -1);
    }
    lua_pop(L, 1);
    return title;
  }

  ~Config(void) {
    if (!(L == nullptr))
      lua_close(L);
  }

private:
  lua_State *L;
};
}; // namespace CliConfig

#endif
