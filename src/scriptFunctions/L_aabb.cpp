#include "../ScriptEngine.h"
#include "../luaEX.h"

#include "../components/AABBCollider.h"

namespace myth
{
    void ScriptEngine::aabb_openlibs()
    {
        luaL_Reg aabbObject[] =
        {
            {"getPos", s_aabb_getpos},
            {"setPos", s_aabb_setpos},
            {"getExtents", s_aabb_getextents},
            {"setExtents", s_aabb_setextents},
            {nullptr, nullptr}
        };

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.aabb");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, aabbObject, 0);
        }
        lua_setglobal(L, "AABB");
    }

    int ScriptEngine::s_aabb_getpos(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.aabb");
        luaL_argcheck(L, ud != NULL, 1, "`aabb' expected");

        AABBCollider *a = *(AABBCollider**)ud;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *b = (m3d::vec3*)lua_newuserdata(L, nbytes);
        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *b = a->getOffset();

        return 1;
    }

    int ScriptEngine::s_aabb_setpos(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.aabb");
        luaL_argcheck(L, ud != NULL, 1, "`aabb' expected");

        AABBCollider *a = *(AABBCollider**)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        a->setOffset(*b);

        return 0;
    }

    int ScriptEngine::s_aabb_getextents(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.aabb");
        luaL_argcheck(L, ud != NULL, 1, "`aabb' expected");

        AABBCollider *a = *(AABBCollider**)ud;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *b = (m3d::vec3*)lua_newuserdata(L, nbytes);
        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *b = a->getExtents();

        return 1;
    }

    int ScriptEngine::s_aabb_setextents(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.aabb");
        luaL_argcheck(L, ud != NULL, 1, "`aabb' expected");

        AABBCollider *a = *(AABBCollider**)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        a->setExtents(*b);

        return 0;
    }
}
