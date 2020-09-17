#pragma once

#include "CollisionComponent.h"
#include <m3d/vec3.h>
#include <TigerEngine/Sphere.h>

namespace myth
{
    class PhysicsEngine;
    class SphereCollider : public CollisionComponent
    {
    private:
        tgr::Sphere m_collider;
        m3d::vec3 m_offset;
    public:
        SphereCollider(const float& radius, const m3d::vec3& offset);

        inline float getRadius() { return m_collider.getRadius(); };
        inline m3d::vec3 getOffset() { return m_offset; };

        inline void setRadius(const float& new_radius) { m_collider.setRadius(new_radius); };
        inline void setOffset(const m3d::vec3& new_offset) { m_offset = new_offset; };

        virtual void fixedUpdate() override;

        inline virtual tgr::Collider *getNativeCollider() override { return &m_collider; };
    };
}

