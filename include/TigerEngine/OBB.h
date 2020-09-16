#pragma once

#include "Collider.h"
#include <m3d/quat.h>

namespace tgr
{
    class OBB : public Collider
    {
    private:
        m3d::vec3 m_center;
        m3d::vec3 m_extents;
        m3d::quat m_rotation;

    public:
        OBB();
        OBB(m3d::vec3 center, m3d::vec3 extents, m3d::quat rotation);

        m3d::vec3 getCenter() const;
        m3d::vec3 getExtents() const;
        m3d::quat getRotation() const;

        OBB& setCenter(const m3d::vec3& center);
        OBB& setExtents(const m3d::vec3& extents);
        OBB& setRotation(const m3d::quat& rotation);

        OBB& translate(const m3d::vec3& translation);

        virtual Collision checkCollision(const Collider* other) const override;
        virtual m3d::vec3 nearestPoint(const m3d::vec3& point) const override;
    };
}
