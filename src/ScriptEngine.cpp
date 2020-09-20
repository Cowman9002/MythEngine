#include "ScriptEngine.h"

#include "luaEX.h"
#include "scriptFunctions/LuaScripts.h"

#include <m3d/m3d.h>
#include <DragonEngine/Camera.h>

namespace myth
{
    ScriptEngine *ScriptEngine::s_bound;

    void setInputNamespace(lua_State *L, luaL_Reg* funcs);

    bool ScriptEngine::initialize(dgn::Window *window, RenderingEngine* render, ResourceManager *resources)
    {
        if(!window || !render || !resources) return false;

        m_window = window;
        m_render = render;
        m_resources = resources;

        L = luaL_newstate();
        luaL_openlibs(L);

        l_vec3_openlibs(L);
        l_rigidbody_openlibs(L);
        l_quat_openlibs(L);
        material_openlibs();
        camera_openlibs();
        entity_openlibs();
        l_transform_openlibs(L);
        sphere_openlibs();
        aabb_openlibs();
        input_openlibs();

        lua_newtable(L);
        {
            lua_pushstring(L, "frame");
            lua_pushnumber(L, 0);
            lua_settable(L, -3);
        }
        lua_setglobal(L, "Myth");

        ScriptEngine::s_bound = this;

        return true;
    }

    void ScriptEngine::terminate()
    {
        lua_settop(L, 0);
        lua_close(L);
    }

    void ScriptEngine::createNewComponent(const unsigned& entity_id, const std::string& name)
    {
        lua_getglobal(L, name.c_str());

        if(!lua_istable(L, -1))
        {
            printf("Error:: Lua definition does not exist for %s\n", name.c_str());
            return;
        }

        lua_getfield(L, -1, "new");

        if(!lua_isfunction(L, -1))
        {
            printf("Error:: no initializer for %s\n", name.c_str());
            return;
        }

        lua_newtable(L);

        if(lua_pcall(L, 1, 1, 0) != LUA_OK)
        {
            print_error(L);
            return;
        }

        const char *instance = (name + std::to_string(entity_id)).c_str();

        //printf("%s created\n", instance);

        lua_setglobal(L, instance);
    }

    void ScriptEngine::callParameterlessMethod(const unsigned& entity_id, const std::string& comp_name, const std::string& method)
    {
        // get component from lua state
        lua_getglobal(L, comp_name.c_str());
        if(!lua_istable(L, -1))
        {
            printf("Error:: component instance does not exist for %s\n", comp_name.c_str());
            return;
        }

        // get method from lua state
        lua_getfield(L, -1, method.c_str());

        if(lua_isfunction(L, -1))
        {
            // push object as a parameter
            lua_getglobal(L, (comp_name + std::to_string(entity_id)).c_str());

            // call method
            if(lua_pcall(L, 1, 0, 0) != LUA_OK)
            {
                print_error(L);
                return;
            }
        }
        else
        {
            //printf("Method %s no exist\n", method.c_str());
        }
        lua_pop(L, 1);
    }

    void ScriptEngine::setNamespaceValue(const std::string& namesp, const std::string& valueName, const double& value)
    {
        lua_getglobal(L, namesp.c_str());
        if(!lua_istable(L, -1))
        {
            printf("Error:: namespace %s does not exist\n", namesp.c_str());
            return;
        }

        lua_pushstring(L, valueName.c_str());
        lua_pushnumber(L, value);
        lua_settable(L, -3);
        lua_pop(L, 1);

        //printf("%i\n", lua_gettop(L));
    }

    void ScriptEngine::callScript(const std::string& filename)
    {
        lua_runfile(L, filename);
    }
}
