#include "CoreEngine.h"

#include <DragonEngine/ErrorString.h>

#include "components/MeshRenderer.h"

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

        if(!m_script_engine.initialize(&m_window, &m_render))
        {
            return false;
        }

        m_initialized = true;

        return true;
    }

    void CoreEngine::terminate()
    {
        m_script_engine.terminate();
        m_scene_graph.dispose();
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
        m_script_engine.callScript("res/scripts/test.lua");


        m_run = true;
        run();
    }

    void CoreEngine::run()
    {
        ////////////////////////////////////
        //          CREATE OBJECTS        //
        ////////////////////////////////////

        dgn::Camera *camera = m_render.getCamera();

        MeshRenderer level_renderer = MeshRenderer(m_resources.getIndex("level"), m_resources.getModel(m_resources.getIndex("level"))->size());
        level_renderer.addMaterial(m_resources.getIndex("brick_mat"));
        level_renderer.addMaterial(m_resources.getIndex("plaster_mat"));
        level_renderer.addMaterial(m_resources.getIndex("paint_wood_mat"));
        level_renderer.addMaterial(m_resources.getIndex("wood_mat"));
        level_renderer.addMaterial(m_resources.getIndex("ground_mat"));
        level_renderer.addMaterial(m_resources.getIndex("concrete_mat"));
        level_renderer.addMaterial(m_resources.getIndex("planks_mat"));
        level_renderer.addMaterial(m_resources.getIndex("brick_mat"));

        Entity level;
        level.addComponent(&level_renderer);
        m_scene_graph.addToScene(&level);

        MeshRenderer ball_renderer = MeshRenderer(m_resources.getIndex("ball"), m_resources.getModel(m_resources.getIndex("ball"))->size());
        ball_renderer.addMaterial(m_resources.getIndex("metal_mat"));

        Entity ball = Entity();
        ball.addComponent(&ball_renderer);
        m_scene_graph.addToScene(&ball);

        m_script_engine.createNewComponent(1, "Player");

        float delta = 1.0 / 60.0;

        //////////////////////////////
        //            LOOP          //
        //////////////////////////////

        while(m_run)
        {
            if(m_window.shouldClose())
            {
                m_run = false;
            }

            m_window.getInput().pollEvents();

            //////////////////////////////////
            //          UPDATE              //
            //////////////////////////////////

            m_script_engine.callParameterlessMethod(1, "Player", "update");

            ball.transform.pos = m3d::vec3(sin(m_window.getFrameCount() / 30.0f) + 1.0f);

            Material *m = m_resources.getMaterial(m_resources.getIndex("brick_mat"));

            m->setUniform(m_resources.getShaderUniformLocation(m->getShader(), "uColorMix"),
                          m3d::vec3(std::max(1.0f, tanf(m_window.getFrameCount() / 30.0f))));

            m_scene_graph.render(&m_render);

            m_render.drawAll();
            m_window.swapBuffers();
        }
    }
}
