#include <iostream>

extern "C" {
#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
}

int main(void) {
  lua_State *state = luaL_newstate();
  std::string color;
  luaL_openlibs(state);

  if (luaL_dofile(state, "config.lua") != LUA_OK) {
    lua_pop(state, 1);
    lua_close(state);
    return 1;
  }

  lua_getglobal(state, "PS1");
  if (lua_istable(state, -1)) {
    lua_getfield(state, -1, "color");

    if (lua_isstring(state, -1)) {
      color = lua_tostring(state, -1);
    }

    lua_pop(state, 1);
  } else {
    color = "undefined";
  }
  lua_pop(state, 1);

  std::cout << "color = " << color << '\n';

  lua_close(state);
  return 0;
}
