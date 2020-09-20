#pragma once
#include <DragonEngine/Mesh.h>
#include <DragonEngine/Texture.h>
#include <DragonEngine/Shader.h>
#include "Material.h"

#include <vector>
#include <unordered_map>

namespace myth
{
    struct TextureData
    {
        std::string filepath = "";
        std::string name = "";
        dgn::TextureType type = dgn::TextureType::Texture2D;
        dgn::TextureWrap wrap = dgn::TextureWrap::Repeat;
        dgn::TextureFilter filter = dgn::TextureFilter::Trilinear;
        dgn::TextureStorage i_storage = dgn::TextureStorage::SRGBA;
        float anisotrpy = -1.0f;
    };

    struct ModelData
    {
        std::string filepath = "";
        std::string name = "";
    };

    struct ShaderData
    {
        std::string v_filepath = "";
        std::string g_filepath = "";
        std::string f_filepath = "";
        std::string name = "";
    };

    struct MaterialData
    {
        std::string filepath = "";
        std::string name = "";
    };

    class ResourceManager
    {
    private:
        std::vector<dgn::Model> m_models;
        std::vector<dgn::Texture> m_textures;
        std::vector<dgn::Shader> m_shaders;
        std::vector<Material> m_materials;

        std::unordered_map<std::string, unsigned> m_index_map;
    public:

        void loadDebugObjects();

        unsigned loadModel(const ModelData& data);
        unsigned loadTexture(const TextureData& data);
        unsigned loadShader(const ShaderData& data);
        unsigned loadMaterial(const MaterialData& data);

        unsigned createModel(const std::vector<float>& vertices, const std::vector<unsigned> indices, const std::string& name);

        unsigned addMaterial(const Material& material, const std::string& name);

        dgn::Model *getModel(const unsigned& index);
        dgn::Texture *getTexture(const unsigned& index);
        dgn::Shader *getShader(const unsigned& index);
        Material *getMaterial(const unsigned& index);

        unsigned getIndex(const std::string& name) const;

        int getShaderUniformLocation(const unsigned& index, const char *name);

        void dispose();
    };

}
