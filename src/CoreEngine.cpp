#include "CoreEngine.h"

#include <DragonEngine/ErrorString.h>

#include "components/MeshRenderer.h"
#include "components/ScriptComponent.h"

#include <cmath>

namespace myth
{
    bool CoreEngine::initialize(const unsigned& width, const unsigned& height)
    {
        if(!m_window.initialize(width, height, "Myth"))
        {
            return false;
        }
        m_window.setVsync(dgn::VsyncMode::Single);
        m_window.makeCurrent();

        if(!m_render.initialize(&m_window.getRenderer(), &m_resources))
        {
            return false;
        }

        m_render.getCamera()->width = width;
        m_render.getCamera()->height = height;

        if(!m_physics.initialze("res/scripts/physics.lua")) return false;

        if(!m_script_engine.initialize(&m_window, &m_render, &m_resources)) return false;

        if(!m_scene_loader.initialize(&m_scene_graph, &m_resources, &m_script_engine, &m_render, &m_physics)) return false;

        m_initialized = true;

        return true;
    }

    void CoreEngine::terminate()
    {
        m_scene_graph.dispose();
        m_script_engine.terminate();
        m_resources.dispose();
        m_window.terminate();
    }

    void CoreEngine::start()
    {
        //////////////////////////////////
        //        LOAD RESOURCES        //
        //////////////////////////////////

        m_resources.loadDebugObjects();

        m_scene_loader.loadResources("res/scenes/simple.res");

        const char* error;
        while((error = dgn::getErrorString()) != nullptr)
        {
            printf("%s", error);
        }

        m_scene_loader.loadScene("res/scenes/simple.scn");

        m_run = true;
        run();
    }

    void CoreEngine::run()
    {
        float delta = 1.0 / 60.0;
        m_scene_graph.start();

        while(m_run)
        {
            m_window.getInput().pollEvents();
            if(m_window.shouldClose())
            {
                m_run = false;
                break;
            }

            //m3d::vec3 euler = m3d::quat::euler(cam->rotation);
            //printf("%f, %f, %f\n", euler.x * TO_DEGS, euler.y * TO_DEGS, euler.z * TO_DEGS);

            m_script_engine.setNamespaceValue("Myth", "frame", m_window.getFrameCount());
            m_script_engine.setNamespaceValue("Myth", "delta", delta);

            m_scene_graph.update();

            m_scene_graph.fixedUpdate();
            m_physics.simulate();

            m_scene_graph.render(&m_render);
            m_physics.render(m_render, m_resources);

            m_render.drawAll();
            m_window.swapBuffers();
        }
    }
}
