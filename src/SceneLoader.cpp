#include "SceneLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "components/MeshRenderer.h"
#include "components/ScriptComponent.h"

namespace myth
{
    bool SceneLoader::initialize(SceneGraph *sceneGraph, ResourceManager *resources, ScriptEngine *scriptEngine)
    {
        if(sceneGraph == nullptr || resources == nullptr || scriptEngine == nullptr) return false;

        m_sceneGraph = sceneGraph;
        m_resources = resources;
        m_script_engine = scriptEngine;

        return true;
    }

    void SceneLoader::loadScene(const std::string& path)
    {
        std::ifstream file;
        file.open(path);

        if(!file.is_open()) return;

        std::string line;
        Entity *currentEntity;

        while(std::getline(file, line))
        {
            if(line.compare("Entity") == 0)
            {
                currentEntity = m_sceneGraph->createEntity();
                std::cout << currentEntity->getId() << std::endl;
            }
            else
            {
                unsigned colon = line.find(":");
                std::string fun = line.substr(0, colon);
                std::string l_data = line.substr(colon + 1);

                std::cout << fun << "  " << l_data << "\n";

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
                        currentEntity->transform.pos = m3d::vec3
                        (std::stof(data_split[0]), std::stof(data_split[1]), std::stof(data_split[2]));
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
                        currentEntity->transform.rot = m3d::quat
                        (std::stof(data_split[0]), std::stof(data_split[1]), std::stof(data_split[2]), std::stof(data_split[3]));
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

                        MeshRenderer level_renderer = MeshRenderer(model_index, std::stoi(data_split[1]), size);

                        unsigned j = data_split.size();
                        for(unsigned i = 3; i < j; i++)
                        {
                            level_renderer.addMaterial(m_resources->getIndex(data_split[i]));
                        }

                        currentEntity->addComponent(new MeshRenderer(level_renderer));
                    }
                    else
                    {
                        printf("Too few parameters for entity mesh renderer: %s\n", path.c_str());
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
                        printf("Too few parameters for entity name: %s\n", path.c_str());
                    }
                }
            }
        }
    }
}
