#include "ScriptComponent.h"

namespace myth
{
    ScriptComponent::ScriptComponent(const std::string& script, ScriptEngine *engine) :
            Component(ComponentType::Script), m_script_engine(engine), m_script_name(script)
    {

    }

    void ScriptComponent::onAdd()
    {
        m_script_engine->setCurrentEntity(getEntity());
        m_script_engine->createNewComponent(getEntity()->getId(), m_script_name);
    }

    void ScriptComponent::start()
    {
        m_script_engine->setCurrentEntity(getEntity());
        m_script_engine->callParameterlessMethod(getEntity()->getId(), m_script_name, "start");
    }

    void ScriptComponent::update()
    {
        m_script_engine->setCurrentEntity(getEntity());
        m_script_engine->callParameterlessMethod(getEntity()->getId(), m_script_name, "update");
    }
}
