#include "PhysicsEngine.h"

#include "luaEX.h"
#include "scriptFunctions/LuaScripts.h"

#include "RenderingEngine.h"
#include "ResourceManager.h"

#include <TigerEngine/Sphere.h>
#include <TigerEngine/AABB.h>
#include <TigerEngine/Triangle.h>

#include <stdio.h>
#include <cmath>

namespace myth
{
    bool PhysicsEngine::initialze(float delta)
    {
        m_delta = delta;

        return true;
    }

    void PhysicsEngine::terminate()
    {
    }

    void PhysicsEngine::simulate()
    {
        // rigid body stuffs
        for(RigidBody *body : m_rigidbodies)
        {
            if(!body->isStatic())
            {
                body->getEntity()->transform.pos += body->velocity * m_delta;
                body->velocity = m3d::vec3::lerp(body->velocity, m3d::vec3(), body->damping);
                if(m3d::vec3::distanceSqr(body->velocity, m3d::vec3()) < 0.01f)
                {
                    body->velocity = m3d::vec3(0.0f);
                }

                if(body->useGravity())
                {
                    body->velocity += m_gravity * m_delta;
                }
            }
        }

        // COllision
        unsigned k = m_bodies.size();
        for(unsigned i = 0; i < k; i++)
        {
            for(unsigned j = i+1; j < k; j++)
            {
                PhysicsBody a = m_bodies[i];
                PhysicsBody b = m_bodies[j];

                if(a.entity == b.entity) continue;

                tgr::Collision collision = a.collider->checkCollision(b.collider);

                if(collision.hit)
                {
                    m3d::vec3 velocityA = m3d::vec3();
                    m3d::vec3 velocityB = m3d::vec3();

                    float massA = 0.0f;
                    float massB = 0.0f;

                    float bounceA = 0.0f;
                    float bounceB = 0.0f;

                    m3d::vec2 frictionA = m3d::vec2(0.0f);
                    m3d::vec2 frictionB = m3d::vec2(0.0f);

                    if(a.body != nullptr)
                    {
                        velocityA = a.body->velocity;
                        bounceA = a.body->bounce;
                        frictionA = a.body->friction;
                        if(a.body->mass != 0.0f)
                        {
                            massA = 1.0f / a.body->mass;
                        }
                    }
                    if(b.body)
                    {
                        velocityB = b.body->velocity;
                        bounceB = b.body->bounce;
                        frictionB = b.body->friction;
                        if(b.body->mass != 0.0f)
                        {
                            massB = 1.0f / b.body->mass;
                        }
                    }

                    m3d::vec3 velocity = velocityB - velocityA;
                    float velOnorm = collision.normal.dot(velocity);

                    if(velOnorm >= 0)
                    {
                        float e = (bounceA + bounceB) * 0.5f;

                        float j = -(1 + e) * velOnorm;
                        j /= massA + massB;
                        m3d::vec3 impulse = collision.normal * j;

                        velocityA -= impulse * massA;
                        velocityB += impulse * massB;

                        m3d::vec3 tangent = (velocity - collision.normal * velocity.dot(collision.normal)).normalized();

                        float jt = velocity.dot(tangent);
                        jt /= massA + massB;
                        float mu = std::sqrt(frictionA.x * frictionA.x +
                                             frictionB.x * frictionB.x);
                        m3d::vec3 fimpulse = m3d::vec3();

                        if(std::abs(jt) < j * mu)
                        {
                            fimpulse = tangent * jt;
                        }
                        else
                        {
                            float dmu = std::sqrt(frictionA.y * frictionA.y +
                                                    frictionB.y * frictionB.y);
                            fimpulse = tangent * -jt * dmu;
                        }

                        velocityA -= fimpulse * massA;
                        velocityB += fimpulse * massB;

                        if(a.body != nullptr)
                        {
                            a.body->velocity = velocityA;
                        }
                        if(b.body)
                        {
                            b.body->velocity = velocityB;
                        }
                    }

                    float percent = 0.8; // .2 to .8
                    float slop = 0.02; // 0.01 to 0.1

                    m3d::vec3 correction = collision.normal * (std::max(collision.penetration - slop, 0.0f) / (massA + massB) * percent);
                    a.entity->transform.pos += correction * massA;
                    b.entity->transform.pos -= correction * massB;
                }
            }
        }
    }

    const float sphere_subdivisions = 16;
    const float sphere_angle = 2.0f * PI / sphere_subdivisions;

    void PhysicsEngine::render(RenderingEngine& render, const ResourceManager& resources)
    {
        for(const PhysicsBody& body : m_bodies)
        {
            // Mesh collider

            /*if(collider->getType() == ComponentType::MeshCollider)
            {
                MeshCollider *m = (MeshCollider*)collider;

                RenderObject ro;

                ro.material = resources.getIndex("debugmesh_mat");
                ro.model = m->getModelIndex();
                ro.sub_mesh = 0;

                render.add(ro);
                continue;
            }*/

            tgr::Collider *col = body.collider;

            switch(col->getType())
            {
                case tgr::ColliderType::Sphere:
                {
                    tgr::Sphere *sphere = (tgr::Sphere*)col;

                    Transform t;
                    t.pos = sphere->getCenter();
                    t.scale = m3d::vec3(sphere->getRadius());

                    RenderObject ro;

                    ro.material = resources.getIndex("debug_wire_mat");
                    ro.model = resources.getIndex("debug_wire_sphere");
                    ro.sub_mesh = 0;
                    ro.transform = t;

                    render.add(ro);
                    break;
                }
                case tgr::ColliderType::AABB:
                {
                    tgr::AABB *aabb = (tgr::AABB*)col;

                    Transform t;
                    t.pos = aabb->getCenter();
                    t.scale = m3d::vec3(aabb->getExtents());

                    RenderObject ro;

                    ro.material = resources.getIndex("debug_wire_mat");
                    ro.model = resources.getIndex("debug_wire_cube");
                    ro.sub_mesh = 0;
                    ro.transform = t;

                    render.add(ro);
                    break;
                }
                default:
                break;
            }
        }
    }

}
