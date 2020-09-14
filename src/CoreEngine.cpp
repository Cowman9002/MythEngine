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

        if(!m_script_engine.initialize(&m_window, &m_render)) return false;

        if(!m_scene_loader.initialize(&m_scene_graph, &m_resources, &m_script_engine)) return false;

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

        myth::TextureData t_data;

        t_data.filepath = "res/textures/bricks_1/color.png";
        t_data.name = "bricks_texture";
        m_resources.loadTexture(t_data);

        t_data.filepath = "res/textures/concrete_1/color.png";
        t_data.name = "concrete_texture";
        m_resources.loadTexture(t_data);

        t_data.filepath = "res/textures/ground_1/color.png";
        t_data.name = "ground_texture";
        m_resources.loadTexture(t_data);

        t_data.filepath = "res/textures/paint_wood_1/color.png";
        t_data.name = "paint_wood_texture";
        m_resources.loadTexture(t_data);

        t_data.filepath = "res/textures/paint_plaster_1/color.png";
        t_data.name = "plaster_texture";
        m_resources.loadTexture(t_data);

        t_data.filepath = "res/textures/planks_1/color.png";
        t_data.name = "planks_texture";
        m_resources.loadTexture(t_data);

        t_data.filepath = "res/textures/wood_1/color.png";
        t_data.name = "wood_texture";
        m_resources.loadTexture(t_data);

        t_data.filepath = "res/textures/metal_plates_1/color.png";
        t_data.name = "metal_texture";
        m_resources.loadTexture(t_data);

        myth::ModelData m_data;

        m_data.filepath = "res/models/forest_level.obj";
        m_data.name = "level";
        m_resources.loadModel(m_data);

        m_data.filepath = "res/models/ball.obj";
        m_data.name = "ball";
        m_resources.loadModel(m_data);

        myth::ShaderData s_data;

        s_data.v_filepath = "res/shaders/textured.vert";
        s_data.f_filepath = "res/shaders/textured.frag";
        s_data.name = "textured_lit";
        m_resources.loadShader(s_data);

        MaterialData mat_data;

        mat_data.filepath = "res/materials/bricks.mat";
        mat_data.name = "brick_mat";
        m_resources.loadMaterial(mat_data);

        mat_data.filepath = "res/materials/plaster.mat";
        mat_data.name = "plaster_mat";
        m_resources.loadMaterial(mat_data);

        mat_data.filepath = "res/materials/planks.mat";
        mat_data.name = "planks_mat";
        m_resources.loadMaterial(mat_data);

        mat_data.filepath = "res/materials/concrete.mat";
        mat_data.name = "concrete_mat";
        m_resources.loadMaterial(mat_data);

        mat_data.filepath = "res/materials/ground.mat";
        mat_data.name = "ground_mat";
        m_resources.loadMaterial(mat_data);

        mat_data.filepath = "res/materials/paint_wood.mat";
        mat_data.name = "paint_wood_mat";
        m_resources.loadMaterial(mat_data);

        mat_data.filepath = "res/materials/wood.mat";
        mat_data.name = "wood_mat";
        m_resources.loadMaterial(mat_data);

        mat_data.filepath = "res/materials/metal.mat";
        mat_data.name = "metal_mat";
        m_resources.loadMaterial(mat_data);

        const char* error;
        while((error = dgn::getErrorString()) != nullptr)
        {
            printf("%s", error);
        }

        // add scripts
        //TODO: figure out what to do with this
        m_script_engine.callScript("res/scripts/player.lua");
        m_script_engine.callScript("res/scripts/ball.lua");

        m_scene_loader.loadScene("res/scenes/simple.scn");

        m_run = true;
        run();
    }

    void CoreEngine::run()
    {
        ////////////////////////////////////
        //          CREATE OBJECTS        //
        ////////////////////////////////////

        dgn::Camera *camera = m_render.getCamera();

        camera->position = m3d::vec3(0.0f, 5.0f, 5.0f);
        camera->rotation = m3d::quat(-30 * TO_RADS, m3d::vec3(1.0f, 0.0f, 0.0f));

        float delta = 1.0 / 60.0;
        m_scene_graph.start();

        //////////////////////////////
        //            LOOP          //
        //////////////////////////////

        while(m_run)
        {
            m_window.getInput().pollEvents();
            if(m_window.shouldClose())
            {
                m_run = false;
                break;
            }

            //////////////////////////////////
            //          UPDATE              //
            //////////////////////////////////

            Material *m = m_resources.getMaterial(m_resources.getIndex("brick_mat"));

            m->setUniform(m_resources.getShaderUniformLocation(m->getShader(), "uColorMix"),
                          m3d::vec3(std::max(1.0f, tanf(m_window.getFrameCount() / 30.0f))));

            m_script_engine.setNamespaceValue("Myth", "frame", m_window.getFrameCount());
            m_scene_graph.update();

            m_scene_graph.render(&m_render);

            m_render.drawAll();
            m_window.swapBuffers();
        }
    }
}
