#pragma once

#include "EntityComponent.h"
#include "ResourceManager.h"
#include "ScriptEngine.h"
#include "RenderingEngine.h"

namespace myth
{
    class SceneLoader
    {
    private:
        SceneGraph *m_sceneGraph;
        ResourceManager *m_resources;
        ScriptEngine *m_script_engine;
        RenderingEngine *m_render;
    public:

        bool initialize(SceneGraph *sceneGraph, ResourceManager *resources, ScriptEngine *scriptEngine, RenderingEngine *render);

        void loadScene(const std::string& path);
        void loadResources(const std::string& path);
    };
}
