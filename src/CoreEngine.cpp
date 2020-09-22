#include "CoreEngine.h"

#include <DragonEngine/ErrorString.h>

#include "components/MeshRenderer.h"
#include "components/ScriptComponent.h"

#include <cmath>

namespace myth
{
    const float fixed_delta = 1.0f / 60.0f;

    bool CoreEngine::initialize(const unsigned& width, const unsigned& height)
    {
        if(!m_window.initialize(width, height, "Myth"))
        {
            return false;
        }
        m_window.setVsync(dgn::VsyncMode::None);
        m_window.makeCurrent();

        if(!m_resources.initialize()) return false;

        if(!m_render.initialize(&m_window.getRenderer(), &m_resources)) return false;

        m_render.getCamera()->width = width;
        m_render.getCamera()->height = height;

        if(!m_physics.initialze(fixed_delta)) return false;

        if(!m_script_engine.initialize(&m_window, &m_render, &m_resources)) return false;

        if(!m_scene_loader.initialize(&m_scene_graph, &m_resources, &m_script_engine, &m_render, &m_physics)) return false;

        m_initialized = true;

        return true;
    }

    void CoreEngine::terminate()
    {
        m_scene_graph.dispose();
        m_script_engine.terminate();
        m_resources.terminate();
        m_window.terminate();
    }

    void CoreEngine::start()
    {
        //////////////////////////////////
        //        LOAD RESOURCES        //
        //////////////////////////////////

        m_resources.loadDebugObjects();

        m_scene_loader.loadResources("res/scenes/simple.res");

        FontData data;
        data.filepath = "res/fonts/aleo/Aleo-Regular.otf";
        data.name = "Aleo_Regular_font";
        data.font_size = 50;
        m_resources.loadFont(data);

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
        m_scene_graph.start();

        float last = m_window.getTime();
        float now = 0;
        float delta = 0;
        float fixed_timer = 0;
        m_script_engine.setNamespaceValue("Myth", "fixedDelta", fixed_delta);

        while(m_run)
        {
            m_window.getInput().pollEvents();
            if(m_window.shouldClose())
            {
                m_run = false;
                break;
            }

            now = m_window.getTime();
            delta = now - last;
            last = now;

            m_script_engine.setNamespaceValue("Myth", "frame", m_window.getFrameCount());
            m_script_engine.setNamespaceValue("Myth", "delta", delta);

            m_scene_graph.update();

            fixed_timer += delta;
            if(fixed_timer >= fixed_delta)
            {
                fixed_timer -= fixed_delta;
                m_scene_graph.fixedUpdate();
                m_physics.simulate();
            }

            m_scene_graph.render(&m_render);
            m_physics.render(m_render, m_resources);

            m_render.drawAll();

            m_render.drawText("Hello Text!", m_resources.getFont(m_resources.getIndex("Aleo_Regular_font")), m3d::vec3(50.0, 300.0f, 0.0f));

            m_window.swapBuffers();
        }
    }
}
