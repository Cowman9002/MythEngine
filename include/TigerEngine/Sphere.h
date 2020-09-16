#pragma once

#include "Collider.h"

#include <vector>

namespace tgr
{
    class Sphere : public Collider
    {
    private:
        m3d::vec3 m_center;
        float m_radius;
    public:
        Sphere();
        Sphere(const m3d::vec3& center, const float& radius);

        Sphere& genFromPoints(const m3d::vec3 *points, unsigned long int num_points);
        Sphere& genFromPoints(const std::vector<m3d::vec3>& points);

        m3d::vec3 getCenter() const;
        float getRadius() const;

        Sphere& setCenter(const m3d::vec3& new_center);
        Sphere& setRadius(const float& new_radius);

        Sphere& translate(const m3d::vec3& translation);

        virtual Collision checkCollision(const Collider* other) const override;
        virtual m3d::vec3 nearestPoint(const m3d::vec3& point) const override;
    };

}
