#pragma once

#include "Collider.h"

namespace tgr
{
    class Point : public Collider
    {
    private:
        m3d::vec3 m_point;
    public:
        Point();
        Point(const m3d::vec3& point);

        m3d::vec3 getPoint();
        Point& setPoint(const m3d::vec3& point);

        virtual Collision checkCollision(const Collider* other) const override;
        virtual m3d::vec3 nearestPoint(const m3d::vec3& point) const override;
    };
}
