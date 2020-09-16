#pragma once

#include <vector>

#include "Material.h"
#include "Transform.h"

#include <DragonEngine/Camera.h>
#include <DragonEngine/Renderer.h>

namespace myth
{
    struct RenderObject
    {
        myth::Transform transform;
        unsigned material;
        unsigned model;
        unsigned sub_mesh;
    };

    class ResourceManager;
    class RenderingEngine
    {
    private:
        std::vector<RenderObject> m_render_objects;

        dgn::Renderer *m_renderer;
        ResourceManager *m_resources;

        dgn::Camera m_camera;

    public:
        bool initialize(dgn::Renderer *renderer, ResourceManager *resources);

        dgn::Camera *getCamera() { return &m_camera; };

        void add(const RenderObject& ro);

        void drawAll();
    };
}
