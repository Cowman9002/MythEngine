#pragma once

#include <m3d/m3d.h>

namespace myth
{
    class Transform
    {
    public:
        m3d::vec3 pos;
        m3d::vec3 scale;
        m3d::quat rot;

        inline Transform() : Transform(m3d::vec3(), m3d::vec3(1.0f), m3d::quat()) {};
        inline Transform(m3d::vec3 position, m3d::vec3 scale, m3d::quat rotation) :
            pos(position), scale(scale), rot(rotation) {};

        inline m3d::mat4x4 getTransformation() const
        {
            m3d::mat4x4 pos_mat = m3d::mat4x4(1.0f).translate(pos);
            m3d::mat4x4 scale_mat = m3d::mat4x4(1.0f).scale(scale);
            m3d::mat4x4 rot_mat = m3d::mat4x4(1.0f).rotate(rot);

            return pos_mat * rot_mat * scale_mat;
        };
    };
}
