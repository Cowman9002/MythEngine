#include "LuaScripts.h"
#include "../LuaEX.h"


#include "../Components/RigidBody.h"

namespace myth
{
    void l_rigidbody_openlibs(lua_State *L)
    {
        luaL_Reg rigidbodyObject[] =
        {
            {"getVelocity", l_rigidbody_getvelocity},
            {"setVelocity", l_rigidbody_setvelocity},
            {"getMass", l_rigidbody_getmass},
            {nullptr, nullptr}
        };

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.rigidbody");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, rigidbodyObject, 0);
        }
        lua_setglobal(L, "RigidBody");
    }

    int l_rigidbody_getvelocity(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.rigidbody");
        luaL_argcheck(L, ud != NULL, 1, "`rigidbody' expected");

        RigidBody *a = *(RigidBody**)ud;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *b = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *b = a->velocity;

        return 1;
    }

    int l_rigidbody_setvelocity(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.rigidbody");
        luaL_argcheck(L, ud != NULL, 1, "`rigidbody' expected");

        RigidBody *a = *(RigidBody**)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");

        m3d::vec3 *b = (m3d::vec3*)ud;

        a->velocity = *b;

        return 0;
    }

    int l_rigidbody_getmass(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.rigidbody");
        luaL_argcheck(L, ud != NULL, 1, "`rigidbody' expected");

        RigidBody *a = *(RigidBody**)ud;

        lua_pushnumber(L, a->mass);

        return 1;
    }
}
