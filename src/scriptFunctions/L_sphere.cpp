#include "../ScriptEngine.h"
#include "../luaEX.h"

#include "../components/SphereCollider.h"

namespace myth
{
    void ScriptEngine::sphere_openlibs()
    {
        luaL_Reg sphereObject[] =
        {
            {"getPos", s_sphere_getpos},
            {"setPos", s_sphere_setpos},
            {"getRadius", s_sphere_getradius},
            {"setRadius", s_sphere_setradius},
            {nullptr, nullptr}
        };

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.sphere");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, sphereObject, 0);
        }
        lua_setglobal(L, "Sphere");
    }

    int ScriptEngine::s_sphere_getpos(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.sphere");
        luaL_argcheck(L, ud != NULL, 1, "`sphere' expected");

        SphereCollider *a = *(SphereCollider**)ud;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *b = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *b = a->getOffset();

        return 1;
    }

    int ScriptEngine::s_sphere_getradius(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.sphere");
        luaL_argcheck(L, ud != NULL, 1, "`sphere' expected");

        SphereCollider *a = *(SphereCollider**)ud;

        lua_pushnumber(L, a->getRadius());

        return 1;
    }

    int ScriptEngine::s_sphere_setpos(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.sphere");
        luaL_argcheck(L, ud != NULL, 1, "`sphere' expected");

        SphereCollider *a = *(SphereCollider**)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");

        m3d::vec3 *b = (m3d::vec3*)ud;

        a->setOffset(*b);

        return 0;
    }

    int ScriptEngine::s_sphere_setradius(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.sphere");
        luaL_argcheck(L, ud != NULL, 1, "`sphere' expected");

        SphereCollider *a = *(SphereCollider**)ud;

        luaL_argcheck(L, lua_isnumber(L, 2), 2, "`number' expected");
        float b = lua_tonumber(L, 2);

        a->setRadius(b);

        return 0;
    }
}
