#pragma once

#include "components/CollisionComponent.h"
#include "components/RigidBody.h"

#include <TigerEngine/Collider.h>

#include <vector>
#include <string>

namespace myth
{
    struct PhysicsBody
    {
        tgr::Collider* collider;
        RigidBody *body;
        Entity *entity;
        //uint64_t layer;
        //std::string tag;
    };

    class RenderingEngine;
    class ResourceManager;
    class PhysicsEngine
    {
    private:
        std::vector<PhysicsBody> m_bodies;
        std::vector<RigidBody*> m_rigidbodies;

        float m_delta = 0.0f;
        m3d::vec3 m_gravity = m3d::vec3(0.0f, -9.8f, 0.0f);
    public:

        bool initialze(float delta);
        void terminate();

        void simulate();

        void render(RenderingEngine& render, const ResourceManager& resources);

        inline void add(const PhysicsBody& body) { m_bodies.push_back(body); };
        inline void add(RigidBody* body) { m_rigidbodies.push_back(body); };

        inline void setGravity(const m3d::vec3& gravity) { m_gravity = gravity; };
    };
}
