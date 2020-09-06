#pragma once
#include <DragonEngine/Mesh.h>
#include <DragonEngine/Texture.h>
#include <DragonEngine/Shader.h>

#include <vector>

struct TextureData
{
    std::string filepath = "";
    dgn::TextureType type = dgn::TextureType::Texture2D;
    dgn::TextureWrap wrap = dgn::TextureWrap::Repeat;
    dgn::TextureFilter filter = dgn::TextureFilter::Trilinear;
    dgn::TextureStorage i_storage = dgn::TextureStorage::SRGBA;
    float anisotrpy = -1.0f;
};

struct ModelData
{
    std::string filepath = "";
};

struct ShaderData
{
    std::string v_filepath = "";
    std::string g_filepath = "";
    std::string f_filepath = "";
};

class ResourceManager
{
private:
    std::vector<dgn::Model> m_models;
    std::vector<dgn::Texture> m_textures;
    std::vector<dgn::Shader> m_shaders;
public:

    unsigned loadModel(const ModelData& data);
    unsigned loadTexture(const TextureData& data);
    unsigned loadShader(const ShaderData& data);

    dgn::Model *getModel(const unsigned& index);
    dgn::Texture *getTexture(const unsigned& index);
    dgn::Shader *getShader(const unsigned& index);

    void dispose();
};
