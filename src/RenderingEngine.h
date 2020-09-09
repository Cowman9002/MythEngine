#pragma once

#include <vector>

#include "Material.h"
#include "Transform.h"

namespace dgn
{
    class Renderer;
    class Camera;
}

namespace myth
{
    struct RenderObject
    {
        myth::Transform *transform;
        myth::Material *material;
        unsigned model;
        unsigned sub_mesh;
    };

    class ResourceManager;
    class RenderingEngine
    {
    private:
        std::vector<RenderObject> m_render_objects;

        dgn::Renderer *m_renderer;
        dgn::Camera *m_camera;
        ResourceManager *m_resources;

    public:
        void init(dgn::Renderer *renderer, dgn::Camera *camera, ResourceManager *resources);

        void add(const RenderObject& ro);

        void drawAll();
    };
}
