#include <DragonEngine/Dragon.h>

#include <m3d/Math1D.h>
#include <m3d/Mat3x3.h>
#include <m3d/Vec2.h>

#include "ResourceManager.h"
#include "Renderer.h"

#include <cmath>
#include <unordered_map>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 520

std::unordered_map<std::string, unsigned> loadGameResources(ResourceManager& resources);

int main(int argc, char *argv[])
{
    dgn::Window window;
    window.initialize(WINDOW_WIDTH, WINDOW_HEIGHT, "Myth");
    window.setVsync(dgn::VsyncMode::Single);
    window.makeCurrent();

    window.getRenderer().setClearColor(0.1f, 0.1f, 0.1f);
    window.getRenderer().enableClearFlag(dgn::ClearFlag::color);
    window.getRenderer().enableClearFlag(dgn::ClearFlag::depth);
    window.getRenderer().enableFlag(dgn::RenderFlag::DepthTest);
    window.getRenderer().enableFlag(dgn::RenderFlag::CullFace);
    window.getRenderer().setClipMode(dgn::ClipMode::ZeroToOne);

    dgn::Camera camera = dgn::Camera(m3d::vec3(), m3d::quat(), WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f, 90 * TO_RADS);

    ResourceManager resources;
    std::unordered_map<std::string, unsigned> resource_map = loadGameResources(resources);

    Renderer renderer;
    renderer.Initialze(&resources);

    int shader_u_mvp = resources.getShader(resource_map["shader"])->getUniformLocation("uMVP");
    //int shader_u_norm_mat = resources.getShader(resource_map["shader"])->getUniformLocation("uNormMat");

    Material level_mat;
    level_mat.texture_ids.push_back(resource_map["bricks"]);
    level_mat.texture_ids.push_back(resource_map["plaster"]);
    level_mat.texture_ids.push_back(resource_map["paint_wood"]);
    level_mat.texture_ids.push_back(resource_map["wood"]);
    level_mat.texture_ids.push_back(resource_map["ground"]);
    level_mat.texture_ids.push_back(resource_map["concrete"]);
    level_mat.texture_ids.push_back(resource_map["planks"]);
    level_mat.texture_ids.push_back(resource_map["bricks"]);

    Material ball_mat;
    ball_mat.texture_ids.push_back(resource_map["bricks"]);

    RenderObject level;
    level.material = level_mat;
    level.model_id = resource_map["level"];
    level.shader_id = resource_map["shader"];

    RenderObject ball;
    ball.material = ball_mat;
    ball.model_id = resource_map["ball"];
    ball.shader_id = resource_map["shader"];

    float delta = 1.0f /60.0f;

    while(!window.shouldClose())
    {
        window.getInput().pollEvents();

        m3d::vec2 cam_rotation = m3d::vec2(window.getInput().getGamepadAxis(0, dgn::GamepadAxis::RightX, 0.1f),
                                           window.getInput().getGamepadAxis(0, dgn::GamepadAxis::RightY, 0.1f)) *
                                           delta * 3.0f;

        camera.rotation = m3d::quat(cam_rotation.x, m3d::vec3::down()) * camera.rotation;
        camera.rotation = m3d::quat(cam_rotation.y, -camera.rotation.getRight()) * camera.rotation;

        m3d::vec3 cam_translation = m3d::vec3(window.getInput().getGamepadAxis(0, dgn::GamepadAxis::LeftX, 0.1f),
                                              window.getInput().getGamepadAxis(0, dgn::GamepadAxis::LeftTrigger, 0.0f) -
                                              window.getInput().getGamepadAxis(0, dgn::GamepadAxis::RightTrigger, 0.0f),
                                              window.getInput().getGamepadAxis(0, dgn::GamepadAxis::LeftY, 0.1f));

        camera.position += camera.rotation * cam_translation * delta * 3.0f;

        window.getRenderer().clear();

        renderer.add(level);
        renderer.add(ball);

        renderer.render(camera, window.getRenderer());

        window.swapBuffers();
    }

    resources.dispose();

    window.terminate();
}

std::unordered_map<std::string, unsigned> loadGameResources(ResourceManager& resources)
{
    std::unordered_map<std::string, unsigned> res;
    TextureData t_data;

    t_data.filepath = "res/textures/bricks_1/color.png";
    res["bricks"] = resources.loadTexture(t_data);

    t_data.filepath = "res/textures/concrete_1/color.png";
    res["concrete"] = resources.loadTexture(t_data);

    t_data.filepath = "res/textures/ground_2/color.png";
    res["ground"] = resources.loadTexture(t_data);

    t_data.filepath = "res/textures/paint_wood_1/color.png";
    res["paint_wood"] = resources.loadTexture(t_data);

    t_data.filepath = "res/textures/paint_plaster_1/color.png";
    res["plaster"] = resources.loadTexture(t_data);

    t_data.filepath = "res/textures/planks_1/color.png";
    res["planks"] = resources.loadTexture(t_data);

    t_data.filepath = "res/textures/wood_1/color.png";
    res["wood"] = resources.loadTexture(t_data);

    ModelData m_data;

    m_data.filepath = "res/models/forest_level.obj";
    res["level"] = resources.loadModel(m_data);

    m_data.filepath = "res/models/ball.obj";
    res["ball"] = resources.loadModel(m_data);

    ShaderData s_data;

    s_data.v_filepath = "res/shaders/textured.vert";
    s_data.f_filepath = "res/shaders/textured.frag";
    res["shader"] = resources.loadShader(s_data);

    const char* error;
    while((error = dgn::getErrorString()) != nullptr)
    {
        printf("%s", error);
    }

    return res;
}
