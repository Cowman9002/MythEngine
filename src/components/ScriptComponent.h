#pragma once

#include "../EntityComponent.h"
#include "../ScriptEngine.h"

#include <string>

namespace myth
{
    class ScriptComponent : public Component
    {
    private:
        ScriptEngine *m_script_engine;
        std::string m_script_name;
    public:
        ScriptComponent(const std::string& script, ScriptEngine *engine);

        virtual void onAdd() override;
        virtual void start() override;
        virtual void update() override;
    };
}

