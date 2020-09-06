#pragma once

#include "ResourceManager.h"

namespace dgn
{
    class Renderer;
    class Camera;
}

struct Material
{
    std::vector<unsigned> texture_ids;
};

struct RenderObject
{
    unsigned model_id;
    unsigned shader_id;
    Material material;
};

class Renderer
{
private:
    ResourceManager *m_resources;
    std::vector<RenderObject> m_render_objects;
public:
    void Initialze(ResourceManager *resources);

    void add(const RenderObject& renderObject);

    void render(dgn::Camera& camera, dgn::Renderer& renderer);
};
