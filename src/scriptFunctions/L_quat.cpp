#include "luaScripts.h"
#include "../luaEX.h"

#include <m3d/m3d.h>

namespace myth
{
    void l_quat_openlibs(lua_State *L)
    {
        luaL_Reg quatObject[] =
        {
            {"angleAxis", l_quat_angleaxis},
            {"getRight", l_quat_getright},
            {"getUp", l_quat_getup},
            {"getForward", l_quat_getforward},
            {"getLeft", l_quat_getleft},
            {"getDown", l_quat_getdown},
            {"getBack", l_quat_getback},
            {"rotate", l_quat_rotate},
            {"mul", l_quat_mul},
            {"lookat", l_quat_lookat},
            {"slerp", l_quat_slerp},
            {nullptr, nullptr}
        };

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.quat");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, quatObject, 0);
        }
        lua_setglobal(L, "Quat");
    }

    int l_quat_angleaxis(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        luaL_argcheck(L, lua_isnumber(L, 1), 1, "`number' expected");
        float a = lua_tonumber(L, 1);

        void *ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        m3d::quat c = m3d::quat(a, *b);

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *d = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int l_quat_getright(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = a->getRight();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int l_quat_getup(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = a->getUp();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int l_quat_getforward(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = a->getForward();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int l_quat_getleft(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = -a->getRight();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int l_quat_getdown(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = -a->getUp();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int l_quat_getback(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = -a->getForward();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int l_quat_rotate(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        m3d::vec3 c = m3d::quat::rotateVec3(*a, *b);

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int l_quat_mul(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        ud = luaL_checkudata(L, 2, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 2, "`quat' expected");
        m3d::quat *b = (m3d::quat*)ud;

        m3d::quat c = m3d::quat::mul(*a, *b);

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *d = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int l_quat_lookat(lua_State *L)
    {
        if(!lua_argcount(L, 3)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 3, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 3, "`vec3' expected");
        m3d::vec3 *c = (m3d::vec3*)ud;

        m3d::quat d = m3d::quat::lookat(*a, *b, *c);

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *e = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *e = d;

        return 1;
    }

    int l_quat_slerp(lua_State *L)
    {
        if(!lua_argcount(L, 3)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        ud = luaL_checkudata(L, 2, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 2, "`quat' expected");
        m3d::quat *b = (m3d::quat*)ud;

        luaL_argcheck(L, lua_isnumber(L, 3), 3, "`number' expected");
        float v = lua_tonumber(L, 3);

        m3d::quat c = m3d::quat::slerp(*a, *b, v);

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *d = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }
}
