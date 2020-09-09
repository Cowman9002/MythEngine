#include "ResourceManager.h"

namespace myth
{

    unsigned ResourceManager::loadModel(const ModelData& data)
    {
        dgn::Model res = dgn::Mesh::loadFromFile(data.filepath);
        m_models.push_back(res);
        return m_models.size();
    }

    unsigned ResourceManager::loadTexture(const TextureData& data)
    {
        dgn::Texture texture;

        switch(data.type)
        {
        case dgn::TextureType::Texture2D:
            {
                texture.loadAs2D(data.filepath, data.wrap, data.filter, data.i_storage, data.anisotrpy);
                break;
            }
        default:
            break;
        }

        m_textures.push_back(texture);
        return m_textures.size();
    }

    unsigned ResourceManager::loadShader(const ShaderData& data)
    {
        dgn::Shader res;
        res.loadFromFiles(data.v_filepath, data.g_filepath, data.f_filepath);
        m_shaders.push_back(res);
        return m_shaders.size();
    }

    dgn::Model *ResourceManager::getModel(const unsigned& index)
    {
        if(index != 0 && index < (m_models.size() + 1))
        {
            return &m_models[index - 1];
        }

        return nullptr;
    }

    dgn::Texture *ResourceManager::getTexture(const unsigned& index)
    {
        if(index != 0 && index < (m_textures.size() + 1))
        {
            return &m_textures[index - 1];
        }

        return nullptr;
    }

    dgn::Shader *ResourceManager::getShader(const unsigned& index)
    {
        if(index != 0 && index < (m_shaders.size() + 1))
        {
            return &m_shaders[index - 1];
        }

        return nullptr;
    }

    int ResourceManager::getShaderUniformLocation(const unsigned& index, const char *name)
    {
        dgn::Shader *s = getShader(index);

        if(s != nullptr)
        {
            return s->getUniformLocation(name);
        }

        return -1;
    }

    void ResourceManager::dispose()
    {
        for(dgn::Model& m : m_models)
        {
            for(dgn::Mesh& mesh : m)
            {
                mesh.dispose();
            }
        }

        for(dgn::Texture& t : m_textures)
        {
            t.dispose();
        }

        for(dgn::Shader& s : m_shaders)
        {
            s.dispose();
        }
    }
}
