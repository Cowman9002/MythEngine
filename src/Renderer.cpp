#include "Renderer.h"

#include <DragonEngine/renderer.h>
#include <DragonEngine/camera.h>

void Renderer::Initialze(ResourceManager *resources)
{
    m_resources = resources;
}

void Renderer::add(const RenderObject& renderObject)
{
    m_render_objects.push_back(renderObject);
}

void Renderer::render(dgn::Camera& camera, dgn::Renderer& renderer)
{
    m3d::mat4x4 view_projection = camera.getProjection() * camera.getView();

    for(RenderObject ro : m_render_objects)
    {
        dgn::Shader *shader = m_resources->getShader(ro.shader_id);
        if(shader)
        {
            renderer.bindShader(*shader);
        }

        dgn::Shader::uniform(0, view_projection);

        unsigned i = 0;
        dgn::Model *model = m_resources->getModel(ro.model_id);
        if(model)
        {
            for(dgn::Mesh mesh : *model)
            {
                renderer.bindMesh(mesh);
                if(i < ro.material.texture_ids.size())
                {
                    renderer.bindTexture(*m_resources->getTexture(ro.material.texture_ids[i++]), 0);
                }

                renderer.drawBoundMesh();
            }
        }
    }

    m_render_objects.clear();
}
