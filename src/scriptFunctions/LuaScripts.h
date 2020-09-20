#pragma once

#include <lua/lua.hpp>

namespace myth
{

    ///////////////////////////////////
    //            VEC3               //
    ///////////////////////////////////

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
    int l_vec3_dot(lua_State *L);
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
    int l_vec3_reflection(lua_State *L);
    int l_vec3_equals(lua_State *L);

    ///////////////////////////////////
    //          QUATERNION           //
    ///////////////////////////////////

    void l_quat_openlibs(lua_State *L);
    int l_quat_angleaxis(lua_State *L);
    int l_quat_getright(lua_State *L);
    int l_quat_getup(lua_State *L);
    int l_quat_getforward(lua_State *L);
    int l_quat_getleft(lua_State *L);
    int l_quat_getdown(lua_State *L);
    int l_quat_getback(lua_State *L);
    int l_quat_rotate(lua_State *L);
    int l_quat_mul(lua_State *L);
    int l_quat_lookat(lua_State *L);
    int l_quat_slerp(lua_State *L);

    //////////////////////////////////
    //          TRANSFORM           //
    //////////////////////////////////

    void l_transform_openlibs(lua_State *L);
    int l_transform_getpos(lua_State *L);
    int l_transform_setpos(lua_State *L);
    int l_transform_getrot(lua_State *L);
    int l_transform_setrot(lua_State *L);
    int l_transform_translate(lua_State *L);

    ///////////////////////////////////
    //          RIGIDBODY            //
    ///////////////////////////////////

    void l_rigidbody_openlibs(lua_State *L);
    int l_rigidbody_getvelocity(lua_State *L);
    int l_rigidbody_setvelocity(lua_State *L);
    int l_rigidbody_getmass(lua_State *L);
}
