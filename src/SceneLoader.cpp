#include "SceneLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "components/MeshRenderer.h"
#include "components/ScriptComponent.h"
#include "components/SphereCollider.h"

namespace myth
{
    bool SceneLoader::initialize(SceneGraph *sceneGraph,
                                 ResourceManager *resources,
                                 ScriptEngine *scriptEngine,
                                 RenderingEngine *render,
                                 PhysicsEngine *physics)
    {
        if(!sceneGraph || !resources || !scriptEngine || !render || !physics) return false;

        m_sceneGraph = sceneGraph;
        m_resources = resources;
        m_script_engine = scriptEngine;
        m_render = render;
        m_physics = physics;

        return true;
    }

    #define ENTITY 1
    #define CAMERA 2

    void SceneLoader::loadScene(const std::string& path)
    {
        std::ifstream file;
        file.open(path);

        if(!file.is_open()) return;

        std::string line;
        Entity *currentEntity;
        dgn::Camera *currentCamera;
        unsigned camera_id = 0;

        unsigned inside_of = 0;

        while(std::getline(file, line))
        {
            if(line.compare("Entity") == 0)
            {
                currentEntity = m_sceneGraph->createEntity();
                //std::cout << currentEntity->getId() << std::endl;
                inside_of = ENTITY;
            }
            if(line.compare("Camera") == 0)
            {
                currentCamera = m_render->getCamera();
                //std::cout << camera_id++ << std::endl;
                inside_of = CAMERA;
            }
            else
            {
                unsigned colon = line.find(":");
                std::string fun = line.substr(0, colon);
                std::string l_data = line.substr(colon + 1);

                //std::cout << fun << "  " << l_data << "\n";

                std::vector<std::string> data_split;

                std::stringstream stream(l_data);
                std::string buff;

                while(std::getline(stream, buff, ','))
                {
                    data_split.push_back(buff);
                }

                if(fun.compare("pos") == 0)
                {
                    if(data_split.size() > 2)
                    {
                        if(inside_of == ENTITY)
                        {
                            currentEntity->transform.pos = m3d::vec3
                            (std::stof(data_split[0]), std::stof(data_split[1]), std::stof(data_split[2]));
                        }
                        else if(inside_of == CAMERA)
                        {
                            currentCamera->position = m3d::vec3
                            (std::stof(data_split[0]), std::stof(data_split[1]), std::stof(data_split[2]));
                        }
                    }
                    else
                    {
                        printf("Too few parameters for entity pos: %s\n", path.c_str());
                    }
                }
                else if(fun.compare("scale") == 0)
                {
                    if(data_split.size() > 2)
                    {
                        currentEntity->transform.scale = m3d::vec3
                        (std::stof(data_split[0]), std::stof(data_split[1]), std::stof(data_split[2]));
                    }
                    else
                    {
                        printf("Too few parameters for entity scale: %s\n", path.c_str());
                    }
                }
                else if(fun.compare("rot") == 0)
                {
                    if(data_split.size() > 3)
                    {
                        if(inside_of == ENTITY)
                        {
                            currentEntity->transform.rot = m3d::quat
                            (std::stof(data_split[0]), std::stof(data_split[1]), std::stof(data_split[2]), std::stof(data_split[3]));
                        }
                        else if(inside_of == CAMERA)
                        {
                            currentCamera->rotation = m3d::quat
                            (std::stof(data_split[0]), std::stof(data_split[1]), std::stof(data_split[2]), std::stof(data_split[3]));
                        }
                    }
                    else
                    {
                        printf("Too few parameters for entity rot: %s\n", path.c_str());
                    }
                }
                else if(fun.compare("name") == 0)
                {
                    if(data_split.size() > 0)
                    {
                        currentEntity->name = data_split[0];
                    }
                    else
                    {
                        printf("Too few parameters for entity name: %s\n", path.c_str());
                    }
                }
                else if(fun.compare("meshrenderer") == 0)
                {
                    if(data_split.size() > 3)
                    {
                        unsigned model_index = m_resources->getIndex(data_split[0]);
                        dgn::Model *m = m_resources->getModel(model_index);

                        if(m == nullptr)
                        {
                            printf("Model %s does not exist\n", data_split[0].c_str());
                            continue;
                        }

                        int size = std::stoi(data_split[2]);
                        size = size < 0 ? m->size() : size;

                        MeshRenderer renderer = MeshRenderer(model_index, std::stoi(data_split[1]), size);

                        unsigned j = data_split.size();
                        for(unsigned i = 3; i < j; i++)
                        {
                            renderer.addMaterial(m_resources->getIndex(data_split[i]));
                        }

                        currentEntity->addComponent(new MeshRenderer(renderer));
                    }
                    else
                    {
                        printf("Too few parameters for mesh renderer component: %s\n", path.c_str());
                    }
                }
                else if(fun.compare("script") == 0)
                {
                    if(data_split.size() > 0)
                    {
                       currentEntity->addComponent(new ScriptComponent(data_split[0], m_script_engine));
                    }
                    else
                    {
                        printf("Too few parameters for script component: %s\n", path.c_str());
                    }
                }
                else if(fun.compare("sphere") == 0)
                {
                    if(data_split.size() > 3)
                    {
                        SphereCollider *col = new SphereCollider(std::stof(data_split[0]),
                                                m3d::vec3(std::stof(data_split[1]),
                                                          std::stof(data_split[2]),
                                                          std::stof(data_split[3])));

                       currentEntity->addComponent(col);
                        m_physics->addCollider(col->getNativeCollider());
                    }
                    else
                    {
                        printf("Too few parameters for sphere component: %s\n", path.c_str());
                    }
                }
            }
        }

        file.close();
    }

    void SceneLoader::loadResources(const std::string& path)
    {
        std::ifstream file;
        file.open(path);

        if(!file.is_open()) return;

        std::string line;

        while(std::getline(file, line))
        {
            if(line.compare("Texture") == 0)
            {
                TextureData tex_data;

                if(std::getline(file, line)) // path
                {
                    tex_data.filepath = line;
                }

                if(std::getline(file, line)) // name
                {
                    tex_data.name = line;
                }

                if(std::getline(file, line)) // type
                {
                    if(line.compare("2D") == 0)
                    {
                        tex_data.type = dgn::TextureType::Texture2D;
                    }
                    else if(line.compare("cube") == 0)
                    {
                        tex_data.type = dgn::TextureType::TextureCube;
                    }
                    else if(line.compare("1D") == 0)
                    {
                        tex_data.type = dgn::TextureType::Texture1D;
                    }
                    else if(line.compare("3D") == 0)
                    {
                        tex_data.type = dgn::TextureType::Texture3D;
                    }
                    else
                    {
                        tex_data.type = dgn::TextureType::Texture2D;
                        printf("Unrecognized texture type %s, defaulting to 2D\n", line.c_str());
                    }
                }

                if(std::getline(file, line)) // wrap
                {
                    if(line.compare("repeat") == 0)
                    {
                        tex_data.wrap = dgn::TextureWrap::Repeat;
                    }
                    else if(line.compare("edgeclamp") == 0)
                    {
                        tex_data.wrap = dgn::TextureWrap::ClampToEdge;
                    }
                    //else if(line.compare("borderclamp"))
                    //{
                    //    tex_data.wrap = dgn::TextureWrap::ClampToBorder;
                    //}
                    else if(line.compare("mirror") == 0)
                    {
                        tex_data.wrap = dgn::TextureWrap::Mirror;
                    }
                    else
                    {
                        tex_data.wrap = dgn::TextureWrap::Repeat;
                        printf("Unrecognized texture wrap %s, defaulting to repeat\n", line.c_str());
                    }
                }

                if(std::getline(file, line)) // filter
                {
                    if(line.compare("trilinear") == 0)
                    {
                        tex_data.filter = dgn::TextureFilter::Trilinear;
                    }
                    else if(line.compare("nearest") == 0)
                    {
                        tex_data.filter = dgn::TextureFilter::Nearest;
                    }
                    else if(line.compare("bilinear") == 0)
                    {
                        tex_data.filter = dgn::TextureFilter::Bilinear;
                    }
                    else if(line.compare("bilinearmip") == 0)
                    {
                        tex_data.filter = dgn::TextureFilter::BilinearMip;
                    }
                    else if(line.compare("nearestmip") == 0)
                    {
                        tex_data.filter = dgn::TextureFilter::NearestMip;
                    }
                    else
                    {
                        tex_data.filter = dgn::TextureFilter::Trilinear;
                        printf("Unrecognized texture filter %s, defaulting to trilinear\n", line.c_str());
                    }
                }

                if(std::getline(file, line)) // istorage
                {
                    if(line.compare("srgba") == 0)
                    {
                        tex_data.i_storage = dgn::TextureStorage::SRGBA;
                    }
                    else if(line.compare("srgb") == 0)
                    {
                        tex_data.i_storage = dgn::TextureStorage::SRGB;
                    }
                    else if(line.compare("rgba") == 0)
                    {
                        tex_data.i_storage = dgn::TextureStorage::RGBA;
                    }
                    else if(line.compare("rgb") == 0)
                    {
                        tex_data.i_storage = dgn::TextureStorage::RGB;
                    }
                    else
                    {
                        tex_data.i_storage = dgn::TextureStorage::SRGBA;
                        printf("Unrecognized texture storage %s, defaulting to SRGBA\n", line.c_str());
                    }
                }

                if(std::getline(file, line)) // aniso
                {
                    tex_data.anisotrpy = std::stof(line);
                }

                m_resources->loadTexture(tex_data);
            }
            else if(line.compare("Model") == 0)
            {
                ModelData mod_data;

                if(std::getline(file, line)) // path
                {
                    mod_data.filepath = line;
                }

                if(std::getline(file, line)) // name
                {
                    mod_data.name = line;
                }

                m_resources->loadModel(mod_data);
            }
            else if(line.compare("Shader") == 0)
            {
                ShaderData sha_data;

                if(std::getline(file, line)) // vpath
                {
                    sha_data.v_filepath = line;
                }
                if(std::getline(file, line)) // gpath
                {
                    sha_data.g_filepath = line;
                }
                if(std::getline(file, line)) // fpath
                {
                    sha_data.f_filepath = line;
                }

                if(std::getline(file, line)) // name
                {
                    sha_data.name = line;
                }

                m_resources->loadShader(sha_data);
            }
            else if(line.compare("Material") == 0)
            {
                MaterialData mat_data;

                if(std::getline(file, line)) // path
                {
                    mat_data.filepath = line;
                }

                if(std::getline(file, line)) // name
                {
                    mat_data.name = line;
                }

                m_resources->loadMaterial(mat_data);
            }
            else if(line.compare("Script") == 0)
            {
                if(std::getline(file, line)) // path
                {
                    m_script_engine->callScript(line);
                }
            }
        }

        file.close();

    }
}
