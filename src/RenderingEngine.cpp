#include "RenderingEngine.h"

#include "ResourceManager.h"
#include <DragonEngine/Renderer.h>
#include <DragonEngine/Camera.h>

namespace myth
{
    bool RenderingEngine::initialize(dgn::Renderer *renderer, ResourceManager *resources)
    {
        m_renderer = renderer;
        m_resources = resources;

        if(m_renderer == nullptr || m_resources == nullptr)
        {
            return false;
        }

        m_camera = dgn::Camera();

        m_renderer->setClearColor(0.5f, 0.5f, 0.58f);
        m_renderer->enableClearFlag(dgn::ClearFlag::color);
        m_renderer->enableClearFlag(dgn::ClearFlag::depth);
        m_renderer->enableFlag(dgn::RenderFlag::DepthTest);
        m_renderer->enableFlag(dgn::RenderFlag::CullFace);
        m_renderer->setClipMode(dgn::ClipMode::ZeroToOne);

        return true;
    }

    void RenderingEngine::add(const RenderObject& ro)
    {
        m_render_objects.push_back(ro);
    }

    void RenderingEngine::drawAll()
    {
        m_renderer->clear();

        for(RenderObject& r : m_render_objects)
        {
            dgn::Model *model = m_resources->getModel(r.model);
            if(model != nullptr)
            {
                Material *material = m_resources->getMaterial(r.material);

                if(material != nullptr)
                {
                    material->update(m_renderer, m_resources, m_camera, r.transform);
                    m_renderer->setDrawMode(material->drawMode);
                    m_renderer->setDepthTest(material->depthTest);
                    if(material->cullFace)
                    {
                        m_renderer->enableFlag(dgn::RenderFlag::CullFace);
                    }
                    else
                    {
                        m_renderer->disableFlag(dgn::RenderFlag::CullFace);
                    }
                }
                m_renderer->bindMesh(model->at(r.sub_mesh));
                m_renderer->drawBoundMesh();
            }
        }

        m_render_objects.clear();
    }
}
