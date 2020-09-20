#pragma once

#include <m3d/vec3.h>

#include <vector>

namespace tgr
{
    enum class ColliderType
    {
        AABB,
        Sphere,
        Plane,
        Triangle,
        OBB,
        Capsule,
        Point,
        Line,
        Ray
    };

    struct Collision
    {
        bool hit = false;
        m3d::vec3 point = m3d::vec3();
        float penetration = 0;
        m3d::vec3 normal = m3d::vec3();
    };

    class Collider
    {
    protected:
        const ColliderType type;

        inline Collider(const ColliderType& type) : type(type) {};

    public:

        inline ColliderType getType() const { return type; };

        virtual Collision checkCollision(const Collider* other) const = 0;
        virtual m3d::vec3 nearestPoint(const m3d::vec3& point) const = 0;

    };
}

/*
    Collision information from
    https://gdbooks.gitbooks.io/3dcollisions/content/

*/


/*
        Collision res;

        switch(other->getType())
        {
        case ColliderType::AABB:
            {
                AABB *b = (AABB*)other;
                break;
            }
        case ColliderType::Sphere:
            {
                Sphere *b = (Sphere*)other;
                break;
            }
        case ColliderType::Plane:
            {
                Plane *b = (Plane*)other;
                break;
            }
        case ColliderType::Triangle:
            {
                Triangle *b = (Triangle*)other;
                break;
            }
        case ColliderType::OBB:
            {
                OBB *b = (OBB*)other;
                break;
            }
        case ColliderType::Capsule:
            {
                Capsule *b = (Capsule*)other;
                break;
            }
        case ColliderType::Mesh:
            {
                Mesh *b = (Mesh*)other;

                res = b->checkCollision(this);
                res.normal *= -1;

                break;
            }
        case ColliderType::Point:
            {
                Point *b = (Point*)other;
                break;
            }
        case ColliderType::Line:
            {
                Line *b = (Line*)other;
                break;
            }
        case ColliderType::Ray:
            {
                Ray *b = (Ray*)other;
                break;
            }
        }

        return res;
*/




