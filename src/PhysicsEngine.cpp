#include "PhysicsEngine.h"

#include "luaEX.h"
#include "RenderingEngine.h"
#include "ResourceManager.h"

#include <TigerEngine/Sphere.h>

#include <stdio.h>

namespace myth
{
    bool PhysicsEngine::initialze(const std::string& filepath)
    {
        L = luaL_newstate();
        luaL_openlibs(L);

        luaL_Reg collisionObject[] =
        {
            {"collide", s_collision_collide},
            {nullptr, nullptr}
        };

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.collider");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, collisionObject, 0);
        }
        lua_setglobal(L, "Collider");

        // load simulation file into state
        lua_runfile(L, filepath);

        return true;
    }

    void PhysicsEngine::terminate()
    {
        lua_settop(L, 0);
        lua_close(L);
    }

    void PhysicsEngine::simulate()
    {
        unsigned k = m_colliders.size();
        for(unsigned i = 0; i < k; i++)
        {
            for(unsigned j = i+1; j < k; j++)
            {
                // get function from lua state
                lua_getglobal(L, "simulate");
                if(lua_isfunction(L, -1))
                {
                    // push parameters

                    size_t nbytes = sizeof(tgr::Collider*);
                    tgr::Collider **a = (tgr::Collider**)lua_newuserdata(L, nbytes);
                    luaL_getmetatable(L, "Myth.collider");
                    lua_setmetatable(L, -2);

                    tgr::Collider **b = (tgr::Collider**)lua_newuserdata(L, nbytes);
                    luaL_getmetatable(L, "Myth.collider");
                    lua_setmetatable(L, -2);

                    *a = m_colliders.at(i);
                    *b = m_colliders.at(j);

                    // call method
                    if(lua_pcall(L, 2, 0, 0) != LUA_OK)
                    {
                        print_error(L);
                        return;
                    }
                }
            }
        }
    }

    void PhysicsEngine::addCollider(tgr::Collider* col)
    {
        m_colliders.push_back(col);
    }

    const float sphere_subdivisions = 16;
    const float sphere_angle = 2.0f * PI / sphere_subdivisions;

    void PhysicsEngine::render(RenderingEngine& render, const ResourceManager& resources)
    {
        for(tgr::Collider *col : m_colliders)
        {
            if(col->getType() == tgr::ColliderType::Sphere)
            {
                tgr::Sphere *sphere = (tgr::Sphere*)col;

                Transform t;
                t.pos = sphere->getCenter();
                t.scale = m3d::vec3(sphere->getRadius());

                RenderObject ro;

                ro.material = resources.getIndex("debug_wire_mat");
                ro.model = resources.getIndex("debug_wire_sphere");
                ro.sub_mesh = 0;
                ro.transform = t;

                render.add(ro);
            }
        }
    }

    int PhysicsEngine::s_collision_collide(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.collider");
        luaL_argcheck(L, ud != NULL, 1, "`collider' expected");
        tgr::Collider *a = *(tgr::Collider**)ud;

        ud = luaL_checkudata(L, 2, "Myth.collider");
        luaL_argcheck(L, ud != NULL, 2, "`collider' expected");
        tgr::Collider *b = *(tgr::Collider**)ud;

        lua_pushboolean(L, a->checkCollision(b).hit);

        return 1;
    }
}
