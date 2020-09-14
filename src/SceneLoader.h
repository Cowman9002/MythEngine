#pragma once

#include "EntityComponent.h"
#include "ResourceManager.h"
#include "ScriptEngine.h"

namespace myth
{
    class SceneLoader
    {
    private:
        SceneGraph *m_sceneGraph;
        ResourceManager *m_resources;
        ScriptEngine *m_script_engine;
    public:

        bool initialize(SceneGraph *sceneGraph, ResourceManager *resources, ScriptEngine *scriptEngine);

        void loadScene(const std::string& path);
    };
}
