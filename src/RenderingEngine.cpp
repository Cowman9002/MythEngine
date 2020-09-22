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
        m_renderer->enableFlag(dgn::RenderFlag::AlphaBlend);
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

    void RenderingEngine::drawText(std::string text, Font *font, m3d::vec3 position)
    {
        m_renderer->setAlphaBlend(dgn::AlphaFactor::SourceAlpha, dgn::AlphaFactor::OneMinusSourceAlpha);

        m_renderer->setDrawMode(dgn::DrawMode::Triangles);
        m_renderer->setDepthTest(dgn::DepthTest::Always);
        m_renderer->enableFlag(dgn::RenderFlag::CullFace);

        dgn::Model *model = m_resources->getModel(m_resources->getIndex("text_model"));
        dgn::Shader *shader = m_resources->getShader(m_resources->getIndex("text_shader"));

        unsigned uMP = shader->getUniformLocation("uMP");

        m3d::mat4x4 projection = m3d::mat4x4::initOrtho(1080, 0, 560, 0, -1, 1);
        m3d::mat4x4 transformation = m3d::mat4x4(1.0f);

        m_renderer->bindMesh(model->at(0));
        m_renderer->bindShader(*shader);

        float x_off = position.x;

        for(char c : text)
        {
            FontChar current_char = font->getChar(c);

            transformation.scale(m3d::vec3(current_char.width, current_char.height, 1.0f));
            transformation.translate(m3d::vec3(x_off, position.y, 0.0f));
            x_off += (current_char.advance >> 6);

            dgn::Texture *texture = m_resources->getTexture(current_char.texture_id);
            dgn::Shader::uniform(uMP, projection * transformation);

            if(texture != nullptr) m_renderer->bindTexture(*texture, 0);

            m_renderer->drawBoundMesh();
        }
    }
}
