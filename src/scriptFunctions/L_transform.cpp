#include "LuaScripts.h"
#include "../luaEX.h"

#include "../EntityComponent.h"
#include "../Transform.h"

namespace myth
{
    void l_transform_openlibs(lua_State *L)
    {
        luaL_Reg transformObject[] =
        {
            {"getPos", l_transform_getpos},
            {"setPos", l_transform_setpos},
            {"getRot", l_transform_getrot},
            {"setRot", l_transform_setrot},
            {"translate", l_transform_translate},
            {nullptr, nullptr}
        };

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.transform");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, transformObject, 0);
        }
        lua_setglobal(L, "Transform");
    }

    int l_transform_getpos(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.transform");
        luaL_argcheck(L, ud != NULL, 1, "`transform' expected");

        Transform *a = *(Transform**)ud;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *b = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *b = a->pos;

        return 1;
    }

    int l_transform_setpos(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.transform");
        luaL_argcheck(L, ud != NULL, 1, "`transform' expected");

        Transform *a = *(Transform**)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");

        m3d::vec3 *b = (m3d::vec3*)ud;

        a->pos = *b;

        return 0;
    }

    int l_transform_getrot(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.transform");
        luaL_argcheck(L, ud != NULL, 1, "`transform' expected");

        Transform *a = *(Transform**)ud;

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *b = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *b = a->rot;

        return 1;
    }

    int l_transform_setrot(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.transform");
        luaL_argcheck(L, ud != NULL, 1, "`transform' expected");

        Transform *a = *(Transform**)ud;

        ud = luaL_checkudata(L, 2, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 2, "`quat' expected");

        m3d::quat *b = (m3d::quat*)ud;

        a->rot = *b;

        return 0;
    }

    int l_transform_translate(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.transform");
        luaL_argcheck(L, ud != NULL, 1, "`transform' expected");

        Transform *a = *(Transform**)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");

        m3d::vec3 *b = (m3d::vec3*)ud;

        a->pos += *b;

        return 0;
    }
}
