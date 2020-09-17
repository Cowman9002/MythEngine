#include "PhysicsEngine.h"

#include "luaEX.h"
#include "scriptFunctions/LuaScripts.h"

#include "RenderingEngine.h"
#include "ResourceManager.h"

#include <TigerEngine/Sphere.h>
#include <TigerEngine/AABB.h>

#include <stdio.h>

namespace myth
{
    bool PhysicsEngine::initialze(const std::string& filepath)
    {
        L = luaL_newstate();
        luaL_openlibs(L);

        l_vec3_openlibs(L);
        collider_loadlibs();
        collision_loadlibs();

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
                    CollisionComponent **a = (CollisionComponent**)lua_newuserdata(L, nbytes);
                    luaL_getmetatable(L, "Myth.collider");
                    lua_setmetatable(L, -2);

                    CollisionComponent **b = (CollisionComponent**)lua_newuserdata(L, nbytes);
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

    void PhysicsEngine::addCollider(CollisionComponent* col)
    {
        m_colliders.push_back(col);
    }

    const float sphere_subdivisions = 16;
    const float sphere_angle = 2.0f * PI / sphere_subdivisions;

    void PhysicsEngine::render(RenderingEngine& render, const ResourceManager& resources)
    {
        for(CollisionComponent *collider : m_colliders)
        {
            tgr::Collider *col = collider->getNativeCollider();

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
            else if(col->getType() == tgr::ColliderType::AABB)
            {
                tgr::AABB *aabb = (tgr::AABB*)col;

                Transform t;
                t.pos = aabb->getCenter();
                t.scale = m3d::vec3(aabb->getExtents());

                RenderObject ro;

                ro.material = resources.getIndex("debug_wire_mat");
                ro.model = resources.getIndex("debug_wire_cube");
                ro.sub_mesh = 0;
                ro.transform = t;

                render.add(ro);
            }
        }
    }

}
