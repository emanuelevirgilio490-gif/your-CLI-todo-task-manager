#ifndef CLI_CONFIG_HPP
#define CLI_CONFIG_HPP

#include "cli-colors.hpp"
#include "defines.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>

extern "C" {
#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
}

namespace CliConfig {
class Config {
public:
  Config(void) {
    L = luaL_newstate();
    luaL_openlibs(L);
  }
  bool isStateNULL(void) { return L == NULL; }

  int IsConfigOpen(void) {
#ifdef DEBUG_MODE
    std::cout << "Checking if the config is open"
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
    std::cout << "Checking there is a PS1 in the config";
#endif
    // get PS1 table
    lua_getglobal(L, "PS1");
    if (lua_istable(L, -1)) {
      lua_getfield(L, -1, "text");
      lua_pop(L, 1);
    } else if (lua_isnil(L, -1)) {
      lua_pop(L, 1);
      return NO_PS1;
    }

    return EXIT_SUCCESS;
  }

  // call this func after the SearchPS1()
  const std::string GetPS1(void) {
#ifdef DEBUG_MODE
    std::cout << "Getting the PS1...";
#endif

    std::string text{};

    // get PS1 table
    lua_getglobal(L, "PS1");
    if (lua_istable(L, -1)) {
      lua_getfield(L, -2, "text");

      if (lua_isstring(L, -2)) {
        text = lua_tostring(L, -1);
      } else {
        text = PS1_ERROR;
      }

      lua_pop(L, 2);
    }
    return text;
  }

  ~Config(void) {
    if (!(L == nullptr))
      lua_close(L);
  }

private:
  lua_State *L;
};

const std::unordered_map<std::string, std::string> std_config = {
    {"text", "> "}, {"color", "white"}};
}; // namespace CliConfig

#endif
