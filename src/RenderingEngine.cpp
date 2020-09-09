#include "RenderingEngine.h"

#include "ResourceManager.h"
#include <DragonEngine/Renderer.h>
#include <DragonEngine/Camera.h>

namespace myth
{
    void RenderingEngine::init(dgn::Renderer *renderer, dgn::Camera *camera, ResourceManager *resources)
    {
        m_renderer = renderer;
        m_camera = camera;
        m_resources = resources;
    }

    void RenderingEngine::add(const RenderObject& ro)
    {
        m_render_objects.push_back(ro);
    }

    void RenderingEngine::drawAll()
    {
        for(RenderObject& r : m_render_objects)
        {
            dgn::Model *model = m_resources->getModel(r.model);
            if(model != nullptr)
            {
                r.material->update(m_renderer, m_resources, m_camera, r.transform);
                m_renderer->bindMesh(model->at(r.sub_mesh));
                m_renderer->drawBoundMesh();
            }
        }

        m_render_objects.clear();
    }
}
