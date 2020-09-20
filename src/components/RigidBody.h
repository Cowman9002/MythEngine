#pragma once

#include "../EntityComponent.h"

#include <m3d/vec3.h>

namespace myth
{
    #define STATIC_FLAG (1 << 0)
    #define GRAVITY_FLAG (1 << 1)

    class RigidBody : public Component
    {
    private:
        unsigned char m_flags;
    public:
        m3d::vec3 velocity;
        float damping;
        float mass;
        float bounce;

        //  x = static, y = dynamic
        m3d::vec2 friction;

        RigidBody() : Component(ComponentType::RigidBody){};

        inline bool isStatic() { return m_flags & STATIC_FLAG; };
        inline void setStatic(bool is_static)
        {
            if(is_static) m_flags |= STATIC_FLAG;
            else          m_flags &= ~STATIC_FLAG;
        };

        inline bool useGravity() { return m_flags & GRAVITY_FLAG; };
        inline void setUseGravity(bool use_gravity)
        {
            if(use_gravity) m_flags |= GRAVITY_FLAG;
            else            m_flags &= ~GRAVITY_FLAG;
        };
    };
}

