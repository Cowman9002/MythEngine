#pragma once

#include "../EntityComponent.h"

#include <vector>

namespace myth
{
    class MeshRenderer : public Component
    {
    private:
        unsigned m_model;
        unsigned m_start_index;
        unsigned m_num_sub_meshes;
        std::vector<unsigned> m_materials;
    public:
        MeshRenderer(const unsigned& model, const unsigned& start_index, const unsigned& num_sub_meshes) :
            Component(ComponentType::MeshRenderer), m_model(model), m_start_index(start_index), m_num_sub_meshes(num_sub_meshes) {};

        void addMaterial(const unsigned& material);

        virtual void render(RenderingEngine *render) override;
    };
}
