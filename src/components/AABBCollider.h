#pragma once

#include "CollisionComponent.h"
#include <m3d/vec3.h>
#include <TigerEngine/AABB.h>

namespace myth
{
    class PhysicsEngine;
    class AABBCollider : public CollisionComponent
    {
    private:
        tgr::AABB m_collider;
        m3d::vec3 m_offset;
        m3d::vec3 m_extents;
    public:
        AABBCollider(const m3d::vec3& offset, const m3d::vec3& extents);

        inline m3d::vec3 getOffset() { return m_offset; };
        inline m3d::vec3 getExtents() { return m_extents; };

        inline void setOffset(const m3d::vec3& new_offset) { m_offset = new_offset; };
        inline void setExtents(const m3d::vec3& new_extents) { m_extents = new_extents; };

        virtual void fixedUpdate() override;

        inline virtual tgr::Collider *getNativeCollider() override { return &m_collider; };
    };
}


