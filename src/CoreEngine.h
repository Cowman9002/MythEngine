#pragma once

#include "ResourceManager.h"
#include "RenderingEngine.h"
#include "EntityComponent.h"
#include "ScriptEngine.h"
#include "SceneLoader.h"

#include <DragonEngine/Window.h>

//TODO: collision

namespace myth
{
    class CoreEngine
    {
    private:
        ResourceManager m_resources;
        RenderingEngine m_render;
        SceneGraph m_scene_graph;
        ScriptEngine m_script_engine;
        SceneLoader m_scene_loader;

        dgn::Window m_window;

        bool m_initialized;
        bool m_run;
    public:
        bool initialize(const unsigned& width, const unsigned& height);
        void start();
        void terminate();

    private:
        void run();

    };
}
