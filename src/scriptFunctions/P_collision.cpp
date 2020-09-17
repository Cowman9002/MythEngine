#include "../PhysicsEngine.h"
#include "../luaEX.h"

namespace myth
{
    void PhysicsEngine::collision_loadlibs()
    {
        luaL_Reg collisionObject[] =
        {
            {"hit", s_collision_hit},
            {"depth", s_collision_depth},
            {"normal", s_collision_normal},
            {nullptr, nullptr}
        };
        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.collision");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, collisionObject, 0);
        }
        lua_setglobal(L, "Collision");
    }

    int PhysicsEngine::s_collision_hit(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.collision");
        luaL_argcheck(L, ud != NULL, 1, "`collision' expected");
        tgr::Collision *a = (tgr::Collision*)ud;

        lua_pushboolean(L, a->hit);

        return 1;
    }

    int PhysicsEngine::s_collision_depth(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.collision");
        luaL_argcheck(L, ud != NULL, 1, "`collision' expected");
        tgr::Collision *a = (tgr::Collision*)ud;

        lua_pushnumber(L, a->penetration);

        return 1;
    }

    int PhysicsEngine::s_collision_normal(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.collision");
        luaL_argcheck(L, ud != NULL, 1, "`collision' expected");
        tgr::Collision *a = (tgr::Collision*)ud;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *b = (m3d::vec3*)lua_newuserdata(L, nbytes);
        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *b = a->normal;

        return 1;
    }
}
