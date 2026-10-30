#ifndef CLI_CONFIG_HPP
#define CLI_CONFIG_HPP

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
inline lua_State *L = luaL_newstate();

// functions
inline int IsConfigOpen(void) {
  // check
  if (luaL_dofile(L, FILE_NAME.c_str()) != LUA_OK) {
    std::cout << "Errore: " << lua_tostring(L, -1);
    lua_pop(L, 1);
    lua_close(L);
    return CONFIG_NOT_OPENED;
  }

  luaL_openlibs(L);
  return EXIT_SUCCESS;
}

inline int SearchPS1(const std::string FILE_NAME) {
  // get PS1 table
  lua_getglobal(L, "PS1");
  if (lua_istable(L, -1)) {
    lua_getfield(L, -1, "text");
    lua_pop(L, 1);
  } else if (lua_isnil(L, -1)) {
    lua_pop(L, 1);
    return NO_PS1;
  }

  lua_close(L);
  return EXIT_SUCCESS;
}

// call this func after the SearchPS1()
inline const std::string GetPS1(void) {
  std::string text{};

  // get PS1 table
  lua_getglobal(L, "PS1");
  if (lua_istable(L, -1)) {
    lua_getfield(L, -1, "text");

    if (lua_isstring(L, -1)) {
      text = lua_tostring(L, -1);
    } else {
      text = "__ERROR__";
    }

    lua_pop(L, 1);
  }
  return text;
}

inline const std::unordered_map<std::string, std::string> std_config = {
    {"text", ">"}, {"color", "white"}};
}; // namespace CliConfig

#endif
