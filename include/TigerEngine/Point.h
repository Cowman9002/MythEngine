#pragma once

#include "Collider.h"

namespace tgr
{
    class Point : public Collider
    {
    private:
    public:
        Point();

        virtual Collision checkCollision(const Collider* other) const override;
        virtual m3d::vec3 nearestPoint(const m3d::vec3& point) const override;
    };
}
