#include "SphereCollider.h"

#include <TigerEngine/Sphere.h>
#include "../PhysicsEngine.h"

namespace myth
{
    SphereCollider::SphereCollider(const float& radius, const m3d::vec3& offset) : Component(ComponentType::SphereCollider)
    {
        m_collider.setRadius(radius);
        m_offset = offset;
    }

    void SphereCollider::fixedUpdate()
    {
        Entity *e = getEntity();
        m_collider.setCenter(e->transform.pos + e->transform.rot * m_offset);
    }
}
