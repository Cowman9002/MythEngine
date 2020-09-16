#pragma once

#include <string>

void print_error(lua_State* L);
bool lua_argcount(lua_State *L, unsigned exp);
void lua_runfile(lua_State *L, const std::string& filename);
