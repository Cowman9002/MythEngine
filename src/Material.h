#pragma once

#include <unordered_map>

#include <m3d/vec3.h>

#include <DragonEngine/Renderer.h>

namespace dgn
{
    class Camera;
}

namespace myth
{
    enum class UniformType
    {
        Vec3,
        Int,
        MVPMat,
        ModelMat
    };

    class ResourceManager;
    class Transform;
    class Material
    {
    private:
        unsigned m_shader;

        std::unordered_map<int, UniformType> m_uniforms;
        std::unordered_map<int, m3d::vec3> u_vec3;
        std::unordered_map<int, int> u_int;

        std::unordered_map<unsigned, unsigned> u_textures;
    public:
        dgn::DrawMode drawMode = dgn::DrawMode::Triangles;

        inline void setShader(const unsigned& shader) { m_shader = shader; }

        inline unsigned getShader() { return m_shader; }

        void setUniform(const int& location, const m3d::vec3& value);
        void setUniform(const int& location, const int& value);

        void setUniformAs(const int& location, const UniformType& type);

        void setTexture(const unsigned& unit, const unsigned& texture);

        void update(dgn::Renderer *renderer, ResourceManager *resources, const dgn::Camera& camera, const Transform& transform);
    };
}
