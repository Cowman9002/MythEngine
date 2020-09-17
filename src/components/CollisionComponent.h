#pragma once

#include "../EntityComponent.h"

#include <TigerEngine/Collider.h>

namespace myth
{
    class CollisionComponent : public Component
    {
    protected:
        inline CollisionComponent(const ComponentType& type) : Component(type) {};
    public:
        virtual tgr::Collider *getNativeCollider() = 0;
    };
}
