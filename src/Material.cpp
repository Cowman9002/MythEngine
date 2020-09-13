#include "Material.h"

#include <DragonEngine/Renderer.h>
#include <DragonEngine/Camera.h>
#include "Transform.h"
#include "ResourceManager.h"

namespace myth
{
    void Material::setUniform(const int& location, const m3d::vec3& value)
    {
        if(location < 0) return;

        u_vec3[location] = value;
        m_uniforms[location] = UniformType::Vec3;
    }

    void Material::setUniform(const int& location, const int& value)
    {
        if(location < 0) return;

        u_int[location] = value;
        m_uniforms[location] = UniformType::Int;
    }

    void Material::setUniformAs(const int& location, const UniformType& type)
    {
        m_uniforms[location] = type;
    }

    void Material::setTexture(const unsigned& unit, const unsigned& texture)
    {
        if(unit < 0 || unit > 32) return;

        u_textures[unit] = texture;
    }

    void Material::update(dgn::Renderer *renderer, ResourceManager *resources, const dgn::Camera& camera, Transform *transform)
    {
        m3d::mat4x4 MV_mat = camera.getProjection() * camera.getView();

        dgn::Shader *shader = resources->getShader(m_shader);
        if(shader == nullptr)
        {
            return;
        }

        renderer->bindShader(*shader);

        for(auto t : u_textures)
        {
            dgn::Texture *tex = resources->getTexture(t.second);
            if(tex != nullptr)
            {
                renderer->bindTexture(*tex, t.first);
            }
        }

        for(auto m : m_uniforms)
        {
            switch(m.second)
            {
            case UniformType::MVPMat:
                dgn::Shader::uniform(m.first, MV_mat * transform->getTransformation());
                break;
            case UniformType::Vec3:
                dgn::Shader::uniform(m.first, u_vec3[m.first]);
                break;
            case UniformType::Int:
                dgn::Shader::uniform(m.first, u_int[m.first]);
                break;
            }
        }
    }
}
