#pragma once

#include <lua/lua.hpp>

namespace myth
{
    void l_vec3_openlibs(lua_State *L);
    int l_vec3_new(lua_State *L);
    int l_vec3_getx(lua_State *L);
    int l_vec3_gety(lua_State *L);
    int l_vec3_getz(lua_State *L);
    int l_vec3_setx(lua_State *L);
    int l_vec3_sety(lua_State *L);
    int l_vec3_setz(lua_State *L);
    int l_vec3_add(lua_State *L);
    int l_vec3_sub(lua_State *L);
    int l_vec3_mul(lua_State *L);
    int l_vec3_div(lua_State *L);
    int l_vec3_down(lua_State *L);
    int l_vec3_up(lua_State *L);
    int l_vec3_left(lua_State *L);
    int l_vec3_right(lua_State *L);
    int l_vec3_forward(lua_State *L);
    int l_vec3_back(lua_State *L);
    int l_vec3_normalized(lua_State *L);
    int l_vec3_lerp(lua_State *L);
    int l_vec3_slerp(lua_State *L);
    int l_vec3_distancesqr(lua_State *L);
    int l_vec3_distance(lua_State *L);
}
