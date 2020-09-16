#pragma once

#include "Collider.h"

namespace tgr
{
    class Triangle : public Collider
    {
    private:
        m3d::vec3 m_points[3];
    public:
        Triangle();
        Triangle(const m3d::vec3& p0, const m3d::vec3& p1, const m3d::vec3& p2);

        m3d::vec3* getPoints();
        m3d::vec3 getPoint(const unsigned char& index);

        Triangle& setPoint(const unsigned char& index, const m3d::vec3& new_pos);
        Triangle& setPoints(m3d::vec3 const * const points);

        Triangle& translate(const m3d::vec3& translation);

        virtual Collision checkCollision(const Collider* other) const override;
        virtual m3d::vec3 nearestPoint(const m3d::vec3& point) const override;
    };
}
