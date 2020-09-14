#include "MeshRenderer.h"

#include "ResourceManager.h"
#include "RenderingEngine.h"

namespace myth
{
    void MeshRenderer::addMaterial(const unsigned& material)
    {
        if(material > 0)
        {
            m_materials.push_back(material);
        }
    }

    void MeshRenderer::render(RenderingEngine *render)
    {
        myth::RenderObject ro;

        for(unsigned i = m_start_index; i < m_num_sub_meshes; i++)
        {
            ro.transform = &getEntity()->transform;
            ro.model = m_model;
            ro.sub_mesh = i;
            if(i < m_materials.size())
            {
                ro.material = m_materials.at(i);
            }
            render->add(ro);
        }
    }
}
