#pragma once
#include <DragonEngine/Mesh.h>
#include <DragonEngine/Texture.h>
#include <DragonEngine/Shader.h>
#include "Material.h"
#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

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

    struct FontData
    {
        std::string filepath = "";
        unsigned font_size = 12;
        std::string name = "";
    };

    class ResourceManager
    {
    private:
        std::vector<dgn::Model> m_models;
        std::vector<dgn::Texture> m_textures;
        std::vector<dgn::Shader> m_shaders;
        std::vector<Material> m_materials;
        std::vector<Font> m_fonts;

        std::unordered_map<std::string, unsigned> m_index_map;

        FT_Library ft_lib;
    public:

        bool initialize();
        void terminate();

        void loadDebugObjects();

        unsigned loadModel(const ModelData& data);
        unsigned loadTexture(const TextureData& data);
        unsigned loadShader(const ShaderData& data);
        unsigned loadMaterial(const MaterialData& data);
        unsigned loadFont(const FontData& data);

        unsigned addModel(const dgn::Model& Model, const std::string& name);
        unsigned addTexture(const dgn::Texture& texture, const std::string& name);
        unsigned addMaterial(const Material& material, const std::string& name);

        dgn::Model *getModel(const unsigned& index);
        dgn::Texture *getTexture(const unsigned& index);
        dgn::Shader *getShader(const unsigned& index);
        Material *getMaterial(const unsigned& index);
        Font *getFont(const unsigned& index);

        unsigned getIndex(const std::string& name) const;

        int getShaderUniformLocation(const unsigned& index, const char *name);
    };

}
