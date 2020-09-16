#pragma once

#include "Collider.h"

namespace tgr
{
    class Plane : public Collider
    {
    private:
        m3d::vec3 m_normal;
        float m_distance;
    public:
        Plane();
        Plane(const m3d::vec3& normal, const float& distance);

        Plane& genFromPoints(const m3d::vec3& p0, const m3d::vec3& p1, const m3d::vec3& p2);

        m3d::vec3 getNormal() const;
        float getDistance() const;

        Plane& setNormal(const m3d::vec3& normal);
        Plane& setDistance(const float& distance);

        Plane& translate(const float& translation);

        float distanceFrom(const m3d::vec3& point) const;

        virtual Collision checkCollision(const Collider* other) const override;
        virtual m3d::vec3 nearestPoint(const m3d::vec3& point) const override;
    };
}
