#pragma once

#include "Collider.h"

namespace tgr
{
    class Line : public Collider
    {
    private:
        m3d::vec3 m_start, m_end;
    public:
        Line();
        Line(m3d::vec3 start, m3d::vec3 end);

        m3d::vec3 getStart();
        m3d::vec3 getEnd();

        Line& setStart(m3d::vec3 start);
        Line& setEnd(m3d::vec3 end);

        virtual Collision checkCollision(const Collider* other) const override;
        virtual m3d::vec3 nearestPoint(const m3d::vec3& point) const override;
    };
}
