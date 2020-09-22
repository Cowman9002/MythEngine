#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace myth
{
    bool ResourceManager::initialize()
    {
        if (FT_Init_FreeType(&ft_lib))
        {
            printf("Could not init freeType library\n");
            return false;
        }

        return true;
    }

    void ResourceManager::loadDebugObjects()
    {
        ModelData mod;
        ShaderData shd;
        MaterialData mtd;

        mod.filepath = "res/models/wire_sphere.obj";
        mod.name = "debug_wire_sphere";
        loadModel(mod);

        mod.filepath = "res/models/wire_cube.obj";
        mod.name = "debug_wire_cube";
        loadModel(mod);

        shd.v_filepath = "res/shaders/wireframe.vert";
        shd.f_filepath = "res/shaders/wireframe.frag";
        shd.name = "debug_wire_shader";
        loadShader(shd);

        shd.v_filepath = "res/shaders/debugmesh.vert";
        shd.f_filepath = "res/shaders/debugmesh.frag";
        shd.name = "debugmesh_shader";
        loadShader(shd);

        mtd.filepath = "res/materials/wireframe.mat";
        mtd.name = "debug_wire_mat";
        loadMaterial(mtd);

        mtd.filepath = "res/materials/debugmesh.mat";
        mtd.name = "debugmesh_mat";
        loadMaterial(mtd);

        mtd.filepath = "res/materials/debugmesh_lines.mat";
        mtd.name = "debugmesh_lines_mat";
        loadMaterial(mtd);


        // FONT RENDERING

        std::vector<float> font_vertices =
        {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };

        std::vector<unsigned> font_indices =
        {
            0, 2, 1,
            0, 3, 2
        };

        dgn::Mesh font_mesh;
        font_mesh.createFromData(font_vertices, font_indices);
        font_mesh.setVertexSize(2);
        font_mesh.addVertexAttrib(0, 2);
        font_mesh.complete();

        dgn::Model font_model;
        font_model.push_back(font_mesh);
        addModel(font_model, "text_model");

        shd.v_filepath = "res/shaders/text.vert";
        shd.f_filepath = "res/shaders/text.frag";
        shd.name = "text_shader";
        loadShader(shd);
    }

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
            unsigned colon = line.find(":");
            std::string fun = line.substr(0, colon);
            std::string l_data = line.substr(colon + 1);

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
                    else if(data_split[1].compare("ModelMat") == 0)
                    {
                        u_type = UniformType::ModelMat;
                    }
                    else if(data_split[1].compare("CamPos") == 0)
                    {
                        u_type = UniformType::CamPos;
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
            else if(fun.compare("drawmode") == 0)
            {
                if(data_split.size() > 0)
                {
                    if(data_split[0].compare("lines") == 0)
                    {
                        res.drawMode = dgn::DrawMode::Lines;
                    }
                    else if(data_split[0].compare("linestrip") == 0)
                    {
                        res.drawMode = dgn::DrawMode::LineStrip;
                    }
                    else if(data_split[0].compare("lineloop") == 0)
                    {
                        res.drawMode = dgn::DrawMode::LineLoop;
                    }
                }
                else
                {
                    printf("Too few parameters for drawmode in material: %s\n", data.filepath.c_str());
                }
            }
            else if(fun.compare("depthtest") == 0)
            {
                if(data_split.size() > 0)
                {
                    if(data_split[0].compare("always") == 0)
                    {
                        res.depthTest = dgn::DepthTest::Always;
                    }
                    else if(data_split[0].compare("lequal") == 0)
                    {
                        res.depthTest = dgn::DepthTest::LEqual;
                    }
                }
                else
                {
                    printf("Too few parameters for depthtest in material: %s\n", data.filepath.c_str());
                }
            }
            else if(fun.compare("cull") == 0)
            {
                if(data_split.size() > 0)
                {
                    if(data_split[0].compare("none") == 0)
                    {
                        res.cullFace = false;
                    }
                }
                else
                {
                    printf("Too few parameters for depthtest in material: %s\n", data.filepath.c_str());
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

    unsigned ResourceManager::loadFont(const FontData& data)
    {
        Font res;

        FT_Face face;
        int error = FT_New_Face(ft_lib, data.filepath.c_str(), 0, &face);
        if(error)
        {
            //https://www.freetype.org/freetype2/docs/reference/ft2-error_code_values.html
            printf("Failed to load font %s, code: %i\n", data.filepath.c_str(), error);
            return 0;
        }

        FT_Set_Pixel_Sizes(face, 0, data.font_size);

        dgn::Texture::setUnpackAlignment(dgn::TextureByteAlignment::Byte);

        // load ASCII fonts
        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            dgn::Texture t;
            t.createAs2D(face->glyph->bitmap.buffer, dgn::TextureData::Ubyte, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                         dgn::TextureWrap::ClampToEdge, dgn::TextureFilter::Bilinear, dgn::TextureStorage::Red, dgn::TextureStorage::Red);

            // now store character for later use
            FontChar character;
            character.advance = face->glyph->advance.x;
            character.bearing_x = face->glyph->bitmap_left;
            character.bearing_y = face->glyph->bitmap_top;
            character.height = face->glyph->bitmap.rows;
            character.width = face->glyph->bitmap.width;
            character.texture_id = addTexture(t, "");

            res.setChar(c, character);
        }

        dgn::Texture::setUnpackAlignment();
        FT_Done_Face(face);

        m_fonts.push_back(res);

        unsigned index = m_fonts.size();
        m_index_map[data.name] = index;

        return index;
    }

    unsigned ResourceManager::addModel(const dgn::Model& model, const std::string& name)
    {
        m_models.push_back(model);

        unsigned index = m_models.size();
        m_index_map[name] = index;

        return index;
    }

    unsigned ResourceManager::addTexture(const dgn::Texture& texture, const std::string& name)
    {
        m_textures.push_back(texture);

        unsigned index = m_textures.size();
        m_index_map[name] = index;

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

    Font *ResourceManager::getFont(const unsigned& index)
    {
        if(index != 0 && index < (m_fonts.size() + 1))
        {
            return &m_fonts[index - 1];
        }

        return nullptr;
    }

    unsigned ResourceManager::getIndex(const std::string& name) const
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

    void ResourceManager::terminate()
    {
        FT_Done_FreeType(ft_lib);

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
