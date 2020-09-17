#include "AABBCollider.h"

namespace myth
{
    AABBCollider::AABBCollider(const m3d::vec3& offset, const m3d::vec3& extents) : CollisionComponent(ComponentType::AABBCollider)
    {
        m_offset = offset;
        m_extents = extents;
    }

    void AABBCollider::fixedUpdate()
    {
        Entity *e = getEntity();
        m_collider.setCenterExtents(e->transform.pos + e->transform.rot * m_offset, m_extents);
    }
}

