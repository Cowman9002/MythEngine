#pragma once

#include "components/CollisionComponent.h"

#include <lua/lua.hpp>
#include <vector>
#include <string>

namespace myth
{
    class RenderingEngine;
    class ResourceManager;
    class PhysicsEngine
    {
    private:
        std::vector<CollisionComponent*> m_colliders;
        lua_State* L;
    public:

        bool initialze(const std::string& filepath);
        void terminate();

        void simulate();

        void render(RenderingEngine& render, const ResourceManager& resources);

        void addCollider(CollisionComponent* col);

    private:

        void collider_loadlibs();
        static int s_collider_collide(lua_State *L);
        static int s_collider_translate(lua_State *L);

        void collision_loadlibs();
        static int s_collision_hit(lua_State *L);
        static int s_collision_depth(lua_State *L);
        static int s_collision_normal(lua_State *L);
    };
}
