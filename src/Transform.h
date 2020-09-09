#pragma once

#include <m3d/m3d.h>

namespace myth
{
    class Transform
    {
    private:
        m3d::vec3 m_pos;
        m3d::vec3 m_scale;
        m3d::quat m_rot;
    public:
        inline Transform() : Transform(m3d::vec3(), m3d::vec3(1.0f), m3d::quat()) {};
        inline Transform(m3d::vec3 position, m3d::vec3 scale, m3d::quat rotation) :
            m_pos(position), m_scale(scale), m_rot(rotation) {};

        inline Transform& setPosition(const m3d::vec3& position) { m_pos = position; return *this; };
        inline Transform& setScale(const m3d::vec3& scale) { m_scale = scale; return *this; };
        inline Transform& setRotation(const m3d::quat& rotation) { m_rot = rotation; return *this; };

        inline m3d::vec3 getPosition() const { return m_pos; };
        inline m3d::vec3 getScale() const { return m_scale; };
        inline m3d::quat getRotation() const { return m_rot; };

        inline m3d::mat4x4 getTransformation() const
        {
            m3d::mat4x4 pos_mat = m3d::mat4x4(1.0f).translate(m_pos);
            m3d::mat4x4 scale_mat = m3d::mat4x4(1.0f).scale(m_scale);
            m3d::mat4x4 rot_mat = m3d::mat4x4(1.0f).rotate(m_rot);

            return pos_mat * rot_mat * scale_mat;
        };
    };
}
