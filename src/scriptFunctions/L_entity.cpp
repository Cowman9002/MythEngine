#include "../ScriptEngine.h"
#include "../luaEX.h"

#include "../EntityComponent.h"
#include "../Transform.h"
#include "../components/SphereCollider.h"
#include "../components/AABBCollider.h"

namespace myth
{
    void ScriptEngine::entity_openlibs()
    {
        luaL_Reg entityObject[] =
        {
            {"getSelf", s_entity_getself},
            {"getTransform", s_entity_gettransform},
            {"getSphere", s_entity_getsphere},
            {"getAABB", s_entity_getaabb},
            {nullptr, nullptr}
        };

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.entity");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, entityObject, 0);
        }
        lua_setglobal(L, "Entity");
    }

    int ScriptEngine::s_entity_getself(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        if(ScriptEngine::s_bound->m_current_entity == nullptr)
        {
            return luaL_error(L, "There is no entity currently bound!");
        }

        size_t nbytes = sizeof(Entity*);
        Entity **b = (Entity**)lua_newuserdata(L, nbytes);
        luaL_getmetatable(L, "Myth.entity");
        lua_setmetatable(L, -2);

        *b = ScriptEngine::s_bound->m_current_entity;

        return 1;
    }

    int ScriptEngine::s_entity_gettransform(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.entity");
        luaL_argcheck(L, ud != NULL, 1, "`entity' expected");

        Entity *a = *(Entity**)ud;

        size_t nbytes = sizeof(Transform*);
        Transform **b = (Transform**)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.transform");
        lua_setmetatable(L, -2);

        *b = &a->transform;

        return 1;
    }

    int ScriptEngine::s_entity_getsphere(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.entity");
        luaL_argcheck(L, ud != NULL, 1, "`entity' expected");

        Entity *a = *(Entity**)ud;

        size_t nbytes = sizeof(SphereCollider*);
        SphereCollider **b = (SphereCollider**)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.sphere");
        lua_setmetatable(L, -2);

        *b = (SphereCollider*)a->getComponent(ComponentType::SphereCollider);

        return 1;
    }

    int ScriptEngine::s_entity_getaabb(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.entity");
        luaL_argcheck(L, ud != NULL, 1, "`entity' expected");

        Entity *a = *(Entity**)ud;

        size_t nbytes = sizeof(AABBCollider*);
        AABBCollider **b = (AABBCollider**)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.aabb");
        lua_setmetatable(L, -2);

        *b = (AABBCollider*)a->getComponent(ComponentType::AABBCollider);

        return 1;
    }
}
