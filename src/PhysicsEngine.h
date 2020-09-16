#pragma once

#include <TigerEngine/Collider.h>
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
        std::vector<tgr::Collider*> m_colliders;
        lua_State* L;
    public:

        bool initialze(const std::string& filepath);
        void terminate();

        void simulate();

        void render(RenderingEngine& render, const ResourceManager& resources);

        void addCollider(tgr::Collider* col);

    private:
        static int s_collision_collide(lua_State *L);
    };
}
