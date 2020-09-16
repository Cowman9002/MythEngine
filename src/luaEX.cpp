
#include <lua/lua.hpp>
#include <string>

void print_error(lua_State* L)
 {
  // The error message is on top of the stack.
  // Fetch it, print it and then pop it off the stack.
  const char* message = lua_tostring(L, -1);
  puts(message);
  lua_pop(L, 1);
}

bool lua_argcount(lua_State *L, unsigned exp)
{
    unsigned got = lua_gettop(L);
    if(exp != got)
    {
        luaL_error(L, "Error: expected %I arguments, got %I", exp, got);
        return false;
    }
    return true;
}

void lua_runfile(lua_State *L, const std::string& filename)
{
    int result = luaL_loadfile(L, filename.c_str());
    if ( result != LUA_OK )
    {
        print_error(L);
        return;
    }
    result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if ( result != LUA_OK )
    {
        print_error(L);
        return;
    }
}
