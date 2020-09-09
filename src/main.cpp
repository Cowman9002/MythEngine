#include <DragonEngine/Dragon.h>

#include <m3d/Math1D.h>
#include <m3d/Mat3x3.h>
#include <m3d/Vec2.h>

#include "ResourceManager.h"
#include "Transform.h"
#include "Material.h"
#include "RenderingEngine.h"

#include <cmath>
#include <unordered_map>
#include <vector>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 520

std::unordered_map<std::string, unsigned> loadGameResources(myth::ResourceManager& resources);

int main(int argc, char *argv[])
{
    dgn::Window window;
    window.initialize(WINDOW_WIDTH, WINDOW_HEIGHT, "Myth");
    window.setVsync(dgn::VsyncMode::Single);
    window.makeCurrent();

    window.getRenderer().setClearColor(0.3f, 0.3f, 0.3f);
    window.getRenderer().enableClearFlag(dgn::ClearFlag::color);
    window.getRenderer().enableClearFlag(dgn::ClearFlag::depth);
    window.getRenderer().enableFlag(dgn::RenderFlag::DepthTest);
    window.getRenderer().enableFlag(dgn::RenderFlag::CullFace);
    window.getRenderer().setClipMode(dgn::ClipMode::ZeroToOne);

    dgn::Camera camera = dgn::Camera(m3d::vec3(0.0f, 2.0f, 2.0f), m3d::quat(), WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f, 90 * TO_RADS);

    myth::ResourceManager resources;
    std::unordered_map<std::string, unsigned> resource_map = loadGameResources(resources);

    myth::RenderingEngine render;
    render.init(&window.getRenderer(), &camera, &resources);

    myth::Material brick_mat;
    brick_mat.setShader(resource_map["shader"]);
    brick_mat.setUniformAs(resources.getShaderUniformLocation(resource_map["shader"], "uMVP"), myth::UniformType::MVPMat);
    brick_mat.setUniform(resources.getShaderUniformLocation(resource_map["shader"], "uColorMix"), m3d::vec3(1.0f, 1.0f, 1.0f));
    brick_mat.setTexture(0, resource_map["bricks"]);

    myth::Material plaster_mat;
    plaster_mat.setShader(resource_map["shader"]);
    plaster_mat.setUniformAs(resources.getShaderUniformLocation(resource_map["shader"], "uMVP"), myth::UniformType::MVPMat);
    plaster_mat.setUniform(resources.getShaderUniformLocation(resource_map["shader"], "uColorMix"), m3d::vec3(1.0f, 1.0f, 1.0f));
    plaster_mat.setTexture(0, resource_map["plaster"]);

    myth::Material metal_mat;
    metal_mat.setShader(resource_map["shader"]);
    metal_mat.setUniformAs(resources.getShaderUniformLocation(resource_map["shader"], "uMVP"), myth::UniformType::MVPMat);
    metal_mat.setUniform(resources.getShaderUniformLocation(resource_map["shader"], "uColorMix"), m3d::vec3(1.2f, 1.14f, 0.4f));
    metal_mat.setTexture(0, resource_map["metal"]);

    myth::Transform levelPos = myth::Transform();
    myth::Transform ballPos = myth::Transform(m3d::vec3(0.0f, 3.0f, 0.0f), m3d::vec3(1.0f), m3d::quat());

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


        ballPos.setPosition(m3d::vec3(sin(window.getFrameCount() / 30.0f)));

        brick_mat.setUniform(resources.getShaderUniformLocation(resource_map["shader"], "uColorMix"),
                             m3d::vec3(std::max(1.0f, tanf(window.getFrameCount() / 30.0f))));


        window.getRenderer().clear();

        myth::RenderObject ro;
        ro.transform = &levelPos;
        ro.model = resource_map["level"];
        ro.sub_mesh = 0;
        ro.material = &brick_mat;
        render.add(ro);

        ro.sub_mesh = 1;
        ro.material = &plaster_mat;
        render.add(ro);

        ro.transform = &ballPos;
        ro.model = resource_map["ball"];
        ro.sub_mesh = 0;
        ro.material = &metal_mat;
        render.add(ro);

        render.drawAll();

        window.swapBuffers();
    }

    resources.dispose();

    window.terminate();
}

std::unordered_map<std::string, unsigned> loadGameResources(myth::ResourceManager& resources)
{
    std::unordered_map<std::string, unsigned> res;
    myth::TextureData t_data;

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

    t_data.filepath = "res/textures/metal_plates_1/color.png";
    res["metal"] = resources.loadTexture(t_data);

    myth::ModelData m_data;

    m_data.filepath = "res/models/forest_level.obj";
    res["level"] = resources.loadModel(m_data);

    m_data.filepath = "res/models/ball.obj";
    res["ball"] = resources.loadModel(m_data);

    myth::ShaderData s_data;

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
