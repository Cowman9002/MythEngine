#include "../PhysicsEngine.h"
#include "../luaEX.h"

namespace myth
{
    void PhysicsEngine::collider_loadlibs()
    {
        luaL_Reg colliderObject[] =
        {
            {"collide", s_collider_collide},
            {"translate", s_collider_translate},
            {nullptr, nullptr}
        };

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.collider");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, colliderObject, 0);
        }
        lua_setglobal(L, "Collider");
    }

    int PhysicsEngine::s_collider_collide(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.collider");
        luaL_argcheck(L, ud != NULL, 1, "`collider' expected");
        CollisionComponent *a = *(CollisionComponent**)ud;

        ud = luaL_checkudata(L, 2, "Myth.collider");
        luaL_argcheck(L, ud != NULL, 2, "`collider' expected");
        CollisionComponent *b = *(CollisionComponent**)ud;

        tgr::Collision c = a->getNativeCollider()->checkCollision(b->getNativeCollider());

        size_t nbytes = sizeof(tgr::Collision);
        tgr::Collision *d = (tgr::Collision*)lua_newuserdata(L, nbytes);
        luaL_getmetatable(L, "Myth.collision");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int PhysicsEngine::s_collider_translate(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.collider");
        luaL_argcheck(L, ud != NULL, 1, "`collider' expected");
        CollisionComponent *a = *(CollisionComponent**)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        a->getEntity()->transform.pos += *b;

        return 0;
    }
}
