#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace myth
{

    unsigned ResourceManager::loadModel(const ModelData& data)
    {
        dgn::Model res = dgn::Mesh::loadFromFile(data.filepath);
        m_models.push_back(res);

        unsigned index = m_models.size();
        m_index_map[data.name] = index;

        return index;
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

        unsigned index = m_textures.size();
        m_index_map[data.name] = index;

        return index;
    }

    unsigned ResourceManager::loadShader(const ShaderData& data)
    {
        dgn::Shader res;
        res.loadFromFiles(data.v_filepath, data.g_filepath, data.f_filepath);
        m_shaders.push_back(res);

        unsigned index = m_shaders.size();
        m_index_map[data.name] = index;

        return index;
    }

    unsigned ResourceManager::loadMaterial(const MaterialData& data)
    {
        std::ifstream file;
        file.open(data.filepath);

        if(!file.is_open())
        {
            printf("Failed to open material file %s.\n", data.filepath.c_str());
        }

        Material res;
        unsigned shader_index = 0;

        std::string line;

        while(std::getline(file, line))
        {
            unsigned collon = line.find(":");
            std::string fun = line.substr(0, collon);
            std::string l_data = line.substr(collon + 1);

            std::vector<std::string> data_split;

            std::stringstream stream(l_data);
            std::string buff;

            while(std::getline(stream, buff, ';'))
            {
                data_split.push_back(buff);
            }

            if(fun.compare("shader") == 0)
            {
                if(data_split.size() > 0)
                {
                    shader_index = getIndex(data_split[0]);
                    res.setShader(shader_index);
                }
                else
                {
                    printf("Too few parameters for shader in material: %s\n", data.filepath.c_str());
                }
            }
            else if(fun.compare("texture") == 0)
            {
                if(data_split.size() > 1)
                {
                    unsigned t_i = getIndex(data_split[0]);
                    if(t_i == 0)
                    {
                        printf("Texture: %s not found\n", data_split[0].c_str());
                    }

                    res.setTexture(std::stoi(data_split[1]), t_i);
                }
                else
                {
                    printf("Too few parameters for texture in material: %s\n", data.filepath.c_str());
                }
            }
            else if(fun.compare("uniformc") == 0)
            {
                if(data_split.size() > 1)
                {
                    UniformType u_type = UniformType::Int;

                    if(data_split[1].compare("MVPMat") == 0)
                    {
                        u_type = UniformType::MVPMat;
                    }

                    res.setUniformAs(getShaderUniformLocation(shader_index, data_split[0].c_str()), u_type);
                }
                else
                {
                    printf("Too few parameters for uniformc in material: %s\n", data.filepath.c_str());
                }
            }
            else if(fun.compare("uniformv3") == 0)
            {
                if(data_split.size() > 3)
                {
                    res.setUniform(getShaderUniformLocation(shader_index, data_split[0].c_str()),
                                   m3d::vec3(std::stof(data_split[1]), std::stof(data_split[2]), std::stof(data_split[3])));
                }
                else
                {
                    printf("Too few parameters for uniformv3 in material: %s\n", data.filepath.c_str());
                }
            }
            else if(fun.compare("uniformf") == 0)
            {
                if(data_split.size() > 1)
                {
                    res.setUniform(getShaderUniformLocation(shader_index, data_split[0].c_str()), std::stof(data_split[1]));
                }
                else
                {
                    printf("Too few parameters for uniformf in material: %s\n", data.filepath.c_str());
                }
            }
            else if(fun.compare("uniformi") == 0)
            {
                if(data_split.size() > 1)
                {
                    res.setUniform(getShaderUniformLocation(shader_index, data_split[0].c_str()), std::stoi(data_split[1]));
                }
                else
                {
                    printf("Too few parameters for uniformi in material: %s\n", data.filepath.c_str());
                }
            }
            else
            {
                printf("Function: %s not recognized in material: %s", fun.c_str(), data.filepath.c_str());
            }
        }

        file.close();

        m_materials.push_back(res);

        unsigned index = m_materials.size();
        m_index_map[data.name] = index;

        return index;
    }

    unsigned ResourceManager::addMaterial(const Material& material, const std::string& name)
    {
        m_materials.push_back(material);

        unsigned index = m_materials.size();
        m_index_map[name] = index;

        return index;
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

    Material *ResourceManager::getMaterial(const unsigned& index)
    {
        if(index != 0 && index < (m_materials.size() + 1))
        {
            return &m_materials[index - 1];
        }

        return nullptr;
    }

    unsigned ResourceManager::getIndex(const std::string& name)
    {
        auto n = m_index_map.find(name);

        if(n != m_index_map.end())
        {
            return n->second;
        }

        return 0;
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
