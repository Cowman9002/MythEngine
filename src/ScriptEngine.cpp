#include "ScriptEngine.h"

#include "luaEX.h"

#include <m3d/m3d.h>
#include <DragonEngine/Camera.h>

#include "Transform.h"
#include "EntityComponent.h"
#include "components/SphereCollider.h"

namespace myth
{
    static ScriptEngine *s_bound;

    void setInputNamespace(lua_State *L, luaL_Reg* funcs);

    bool ScriptEngine::initialize(dgn::Window *window, RenderingEngine* render, ResourceManager *resources)
    {
        if(!window || !render || !resources) return false;

        m_window = window;
        m_render = render;
        m_resources = resources;

        L = luaL_newstate();
        luaL_openlibs(L);

        luaL_Reg vec3Object[] =
        {
            {"new", s_vec3_new},
            {"getx", s_vec3_getx},
            {"setx", s_vec3_setx},
            {"gety", s_vec3_gety},
            {"sety", s_vec3_sety},
            {"getz", s_vec3_getz},
            {"setz", s_vec3_setz},
            {"add", s_vec3_add},
            {"sub", s_vec3_sub},
            {"mul", s_vec3_mul},
            {"div", s_vec3_div},
            {"down", s_vec3_down},
            {"up", s_vec3_up},
            {"right", s_vec3_right},
            {"left", s_vec3_left},
            {"forward", s_vec3_forward},
            {"back", s_vec3_back},
            {"normalized", s_vec3_normalized},
            {"lerp", s_vec3_lerp},
            {"slerp", s_vec3_slerp},
            {"distSqr", s_vec3_distancesqr},
            {"dist", s_vec3_distance},
            {nullptr, nullptr}
        };

        luaL_Reg quatObject[] =
        {
            {"angleAxis", s_quat_angleaxis},
            {"getRight", s_quat_getright},
            {"getUp", s_quat_getup},
            {"getForward", s_quat_getforward},
            {"getLeft", s_quat_getleft},
            {"getDown", s_quat_getdown},
            {"getBack", s_quat_getback},
            {"rotate", s_quat_rotate},
            {"mul", s_quat_mul},
            {"face", s_quat_face},
            {"lookat", s_quat_lookat},
            {"slerp", s_quat_slerp},
            {nullptr, nullptr}
        };

        luaL_Reg cameraObject[] =
        {
            {"get", s_camera_get},
            {"setPos", s_camera_setpos},
            {"setRot", s_camera_setrot},
            {"getPos", s_camera_getpos},
            {"getRot", s_camera_getrot},
            {nullptr, nullptr}
        };

        luaL_Reg entityObject[] =
        {
            {"getSelf", s_entity_getself},
            {"getTransform", s_entity_gettransform},
            {"getSphere", s_entity_getsphere},
            {nullptr, nullptr}
        };

        luaL_Reg transformObject[] =
        {
            {"getPos", s_transform_getpos},
            {"setPos", s_transform_setpos},
            {"getRot", s_transform_getrot},
            {"setRot", s_transform_setrot},
            {nullptr, nullptr}
        };

        luaL_Reg materialObject[] =
        {
            {"get", s_material_get},
            {"uniformv3", s_material_setuniformv3},
            {nullptr, nullptr}
        };

        luaL_Reg sphereObject[] =
        {
            {"getPos", s_sphere_getPos},
            {"setPos", s_sphere_setPos},
            {"getRadius", s_sphere_getRadius},
            {"setRadius", s_sphere_setRadius},
            {nullptr, nullptr}
        };

        luaL_Reg inputNamespace[] =
        {
            {"getKey", s_input_getkey},
            {"getKeyDown", s_input_getkeydown},
            {"getKeyUp", s_input_getkeyup},
            {"getControllerAxis", s_input_controlleraxis},
            {nullptr, nullptr}
        };

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.vec3");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, vec3Object, 0);
        }
        lua_setglobal(L, "vec3");

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.quat");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, quatObject, 0);
        }
        lua_setglobal(L, "quat");

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.camera");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, cameraObject, 0);
        }
        lua_setglobal(L, "Camera");

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.entity");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, entityObject, 0);
        }
        lua_setglobal(L, "Entity");

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.transform");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, transformObject, 0);
        }
        lua_setglobal(L, "Transform");

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.material");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, materialObject, 0);
        }
        lua_setglobal(L, "Material");

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.sphere");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, sphereObject, 0);
        }
        lua_setglobal(L, "Sphere");

        setInputNamespace(L, inputNamespace);

        lua_newtable(L);
        {
            lua_pushstring(L, "frame");
            lua_pushnumber(L, 0);
            lua_settable(L, -3);
        }
        lua_setglobal(L, "Myth");

        s_bound = this;

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

    void setInputNamespace(lua_State *L, luaL_Reg* funcs)
    {
        lua_newtable(L);
        {
            luaL_setfuncs(L, funcs, 0);

            lua_pushstring(L, "AxisLeftX");
            lua_pushnumber(L, (unsigned)dgn::GamepadAxis::LeftX);
            lua_settable(L, -3);

            lua_pushstring(L, "AxisLeftY");
            lua_pushnumber(L, (unsigned)dgn::GamepadAxis::LeftY);
            lua_settable(L, -3);

            lua_pushstring(L, "AxisRightX");
            lua_pushnumber(L, (unsigned)dgn::GamepadAxis::RightX);
            lua_settable(L, -3);

            lua_pushstring(L, "AxisRightY");
            lua_pushnumber(L, (unsigned)dgn::GamepadAxis::RightY);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyA");
            lua_pushnumber(L, (unsigned)dgn::Key::A);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyB");
            lua_pushnumber(L, (unsigned)dgn::Key::B);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyC");
            lua_pushnumber(L, (unsigned)dgn::Key::C);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyD");
            lua_pushnumber(L, (unsigned)dgn::Key::D);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyE");
            lua_pushnumber(L, (unsigned)dgn::Key::E);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyF");
            lua_pushnumber(L, (unsigned)dgn::Key::F);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyG");
            lua_pushnumber(L, (unsigned)dgn::Key::G);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyH");
            lua_pushnumber(L, (unsigned)dgn::Key::H);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyI");
            lua_pushnumber(L, (unsigned)dgn::Key::I);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyJ");
            lua_pushnumber(L, (unsigned)dgn::Key::J);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyK");
            lua_pushnumber(L, (unsigned)dgn::Key::K);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyL");
            lua_pushnumber(L, (unsigned)dgn::Key::L);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyM");
            lua_pushnumber(L, (unsigned)dgn::Key::M);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyN");
            lua_pushnumber(L, (unsigned)dgn::Key::N);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyO");
            lua_pushnumber(L, (unsigned)dgn::Key::O);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyP");
            lua_pushnumber(L, (unsigned)dgn::Key::P);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyQ");
            lua_pushnumber(L, (unsigned)dgn::Key::Q);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyR");
            lua_pushnumber(L, (unsigned)dgn::Key::R);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyS");
            lua_pushnumber(L, (unsigned)dgn::Key::S);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyT");
            lua_pushnumber(L, (unsigned)dgn::Key::T);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyU");
            lua_pushnumber(L, (unsigned)dgn::Key::U);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyV");
            lua_pushnumber(L, (unsigned)dgn::Key::V);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyW");
            lua_pushnumber(L, (unsigned)dgn::Key::W);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyX");
            lua_pushnumber(L, (unsigned)dgn::Key::X);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyY");
            lua_pushnumber(L, (unsigned)dgn::Key::Y);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyZ");
            lua_pushnumber(L, (unsigned)dgn::Key::Z);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyUp");
            lua_pushnumber(L, (unsigned)dgn::Key::Up);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyDown");
            lua_pushnumber(L, (unsigned)dgn::Key::Down);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyLeft");
            lua_pushnumber(L, (unsigned)dgn::Key::Left);
            lua_settable(L, -3);

            lua_pushstring(L, "KeyRight");
            lua_pushnumber(L, (unsigned)dgn::Key::Right);
            lua_settable(L, -3);
        }
        lua_setglobal(L, "Input");
    }

    ///////////////////////////////////////////
    //            SCRIPT FUNCTIONS           //
    ///////////////////////////////////////////

    ////////////////////////////
    //          VEC3          //
    ////////////////////////////

    int ScriptEngine::s_vec3_new(lua_State *L)
    {
        m3d::vec3 target;

        // constructors
        if(lua_gettop(L) == 1)
        {
            luaL_argcheck(L, lua_isnumber(L, 1), 1, "`number' expected");
            target = m3d::vec3(lua_tonumber(L, 1));
        }
        else if(lua_gettop(L) == 3)
        {
            for(int i = 1; i < 4; i++)
                luaL_argcheck(L, lua_isnumber(L, i), i, "`number' expected");

            target = m3d::vec3(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
        }
        else if(lua_gettop(L) != 0)
        {
            return luaL_error(L, "Error: expected 0, 1, or 3 arguments, got %I", lua_gettop(L));
        }

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = target;

        return 1;
    }

    int ScriptEngine::s_vec3_getx(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");

        m3d::vec3 *a = (m3d::vec3 *)ud;

        lua_pushnumber(L, a->x);

        return 1;
    }

    int ScriptEngine::s_vec3_gety(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");

        m3d::vec3 *a = (m3d::vec3 *)ud;

        lua_pushnumber(L, a->y);

        return 1;
    }

    int ScriptEngine::s_vec3_getz(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");

        m3d::vec3 *a = (m3d::vec3 *)ud;

        lua_pushnumber(L, a->z);

        return 1;
    }

    int ScriptEngine::s_vec3_setx(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");

        m3d::vec3 *a = (m3d::vec3 *)ud;

        luaL_argcheck(L, lua_isnumber(L, 2), 2, "`number' expected");
        float v = lua_tonumber(L, 2);

        a->x = v;

        return 0;
    }

    int ScriptEngine::s_vec3_sety(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");

        m3d::vec3 *a = (m3d::vec3 *)ud;

        luaL_argcheck(L, lua_isnumber(L, 2), 2, "`number' expected");
        float v = lua_tonumber(L, 2);

        a->y = v;

        return 0;
    }

    int ScriptEngine::s_vec3_setz(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");

        m3d::vec3 *a = (m3d::vec3 *)ud;

        luaL_argcheck(L, lua_isnumber(L, 2), 2, "`number' expected");
        float v = lua_tonumber(L, 2);

        a->z = v;

        return 0;
    }

    int ScriptEngine::s_vec3_add(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3 *)ud;

        m3d::vec3 c;

        if(lua_isnumber(L, 2)) // adding number
        {
            float b = lua_tonumber(L, 2);
            c = m3d::vec3::add(*a, b);
        }
        else
        {
            ud = luaL_checkudata(L, 2, "Myth.vec3");
            luaL_argcheck(L, ud != NULL, 2, "`vec3 or number' expected");
            m3d::vec3 *b = (m3d::vec3 *)ud;
            c = m3d::vec3::add(*a, *b);
        }

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        d->x = c.x;
        d->y = c.y;
        d->z = c.z;

        return 1;
    }

    int ScriptEngine::s_vec3_sub(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3 *)ud;

        m3d::vec3 c;

        if(lua_isnumber(L, 2)) // adding number
        {
            float b = lua_tonumber(L, 2);
            c = m3d::vec3::sub(*a, b);
        }
        else
        {
            ud = luaL_checkudata(L, 2, "Myth.vec3");
            luaL_argcheck(L, ud != NULL, 2, "`vec3 or number' expected");
            m3d::vec3 *b = (m3d::vec3 *)ud;
            c = m3d::vec3::sub(*a, *b);
        }

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        d->x = c.x;
        d->y = c.y;
        d->z = c.z;

        return 1;
    }
    int ScriptEngine::s_vec3_mul(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3 *)ud;

        luaL_argcheck(L, lua_isnumber(L, 2), 2, "`number' expected");
        float b = lua_tonumber(L, 2);

        m3d::vec3 c = m3d::vec3::mul(*a, b);

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        d->x = c.x;
        d->y = c.y;
        d->z = c.z;

        return 1;
    }

    int ScriptEngine::s_vec3_div(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3 *)ud;

        luaL_argcheck(L, lua_isnumber(L, 2), 2, "`number' expected");
        float b = lua_tonumber(L, 2);

        m3d::vec3 c = m3d::vec3::div(*a, b);

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        d->x = c.x;
        d->y = c.y;
        d->z = c.z;

        return 1;
    }

    int ScriptEngine::s_vec3_down(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::down();

        return 1;
    }

    int ScriptEngine::s_vec3_up(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::up();

        return 1;
    }

    int ScriptEngine::s_vec3_left(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::left();

        return 1;
    }

    int ScriptEngine::s_vec3_right(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::right();

        return 1;
    }

    int ScriptEngine::s_vec3_forward(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::forwards();

        return 1;
    }

    int ScriptEngine::s_vec3_back(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::back();

        return 1;
    }

    int ScriptEngine::s_vec3_normalized(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3 *)ud;

        m3d::vec3 c = m3d::vec3::normalized(*a);

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        d->x = c.x;
        d->y = c.y;
        d->z = c.z;

        return 1;
    }

    int ScriptEngine::s_vec3_lerp(lua_State *L)
    {
        if(!lua_argcount(L, 3)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        luaL_argcheck(L, lua_isnumber(L, 3), 3, "`number' expected");
        float v = lua_tonumber(L, 3);

        m3d::vec3 c = m3d::vec3::lerp(*a, *b, v);

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_vec3_slerp(lua_State *L)
    {
        if(!lua_argcount(L, 3)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        luaL_argcheck(L, lua_isnumber(L, 3), 3, "`number' expected");
        float v = lua_tonumber(L, 3);

        //TODO: fix slerp method
        m3d::vec3 c = m3d::vec3::slerp(*a, *b, v);

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_vec3_distancesqr(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        lua_pushnumber(L, m3d::vec3::distanceSqr(*a, *b));

        return 1;
    }

    int ScriptEngine::s_vec3_distance(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        lua_pushnumber(L, m3d::vec3::distance(*a, *b));

        return 1;
    }

    //////////////////////////////////
    //          QUATERNION          //
    //////////////////////////////////

    int ScriptEngine::s_quat_angleaxis(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        luaL_argcheck(L, lua_isnumber(L, 1), 1, "`number' expected");
        float a = lua_tonumber(L, 1);

        void *ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        m3d::quat c = m3d::quat(a, *b);

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *d = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_quat_getright(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = a->getRight();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_quat_getup(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = a->getUp();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_quat_getforward(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = a->getForward();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_quat_getleft(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = -a->getRight();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_quat_getdown(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = -a->getUp();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_quat_getback(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        m3d::vec3 c = -a->getForward();

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_quat_rotate(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        m3d::vec3 c = m3d::quat::rotateVec3(*a, *b);

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_quat_mul(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        ud = luaL_checkudata(L, 2, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 2, "`quat' expected");
        m3d::quat *b = (m3d::quat*)ud;

        m3d::quat c = m3d::quat::mul(*a, *b);

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *d = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_quat_face(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        m3d::quat c = m3d::quat::normalized(m3d::quat::face(*a, *b));

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *d = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int ScriptEngine::s_quat_lookat(lua_State *L)
    {
        if(!lua_argcount(L, 3)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 3, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 3, "`vec3' expected");
        m3d::vec3 *c = (m3d::vec3*)ud;

        m3d::quat d = m3d::quat::lookat(*a, *b, *c);

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *e = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *e = d;

        return 1;
    }

    int ScriptEngine::s_quat_slerp(lua_State *L)
    {
        if(!lua_argcount(L, 3)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");
        m3d::quat *a = (m3d::quat*)ud;

        ud = luaL_checkudata(L, 2, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 2, "`quat' expected");
        m3d::quat *b = (m3d::quat*)ud;

        luaL_argcheck(L, lua_isnumber(L, 3), 3, "`number' expected");
        float v = lua_tonumber(L, 3);

        m3d::quat c = m3d::quat::slerp(*a, *b, v);

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *d = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    /////////////////////////////////
    //          CAMERA             //
    /////////////////////////////////

    int ScriptEngine::s_camera_get(lua_State *L)
    {
        size_t nbytes = sizeof(dgn::Camera*);
        dgn::Camera **d = (dgn::Camera**)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.camera");
        lua_setmetatable(L, -2);

        *d = s_bound->m_render->getCamera();

        return 1;
    }

    int ScriptEngine::s_camera_setpos(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.camera");
        luaL_argcheck(L, ud != NULL, 1, "`camera' expected");

        dgn::Camera *a = *(dgn::Camera**)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");

        m3d::vec3 *b = (m3d::vec3 *)ud;

        a->position = *b;

        return 0;
    }

    int ScriptEngine::s_camera_setrot(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.camera");
        luaL_argcheck(L, ud != NULL, 1, "`camera' expected");

        dgn::Camera *a = *(dgn::Camera**)ud;

        ud = luaL_checkudata(L, 2, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 1, "`quat' expected");

        m3d::quat *b = (m3d::quat *)ud;

        a->rotation = *b;

        return 0;
    }

    int ScriptEngine::s_camera_getpos(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.camera");
        luaL_argcheck(L, ud != NULL, 1, "`camera' expected");

        dgn::Camera *a = *(dgn::Camera**)ud;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *b = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *b = a->position;

        return 1;
    }

    int ScriptEngine::s_camera_getrot(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.camera");
        luaL_argcheck(L, ud != NULL, 1, "`camera' expected");

        dgn::Camera *a = *(dgn::Camera**)ud;

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *b = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *b = a->rotation;

        return 1;
    }

    //////////////////////////////////
    //          ENTITY              //
    //////////////////////////////////
    int ScriptEngine::s_entity_getself(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        if(s_bound->m_current_entity == nullptr)
        {
            return luaL_error(L, "There is no entity currently bound!");
        }

        size_t nbytes = sizeof(Entity*);
        Entity **b = (Entity**)lua_newuserdata(L, nbytes);
        luaL_getmetatable(L, "Myth.entity");
        lua_setmetatable(L, -2);

        *b = s_bound->m_current_entity;

        return 1;
    }

    int ScriptEngine::s_entity_gettransform(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.entity");
        luaL_argcheck(L, ud != NULL, 1, "`entity' expected");

        Entity *a = *(Entity**)ud;

        size_t nbytes = sizeof(Transform*);
        Transform **b = (Transform**)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.transform");
        lua_setmetatable(L, -2);

        *b = &a->transform;

        return 1;
    }

    int ScriptEngine::s_entity_getsphere(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.entity");
        luaL_argcheck(L, ud != NULL, 1, "`entity' expected");

        Entity *a = *(Entity**)ud;

        size_t nbytes = sizeof(SphereCollider*);
        SphereCollider **b = (SphereCollider**)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.sphere");
        lua_setmetatable(L, -2);

        *b = (SphereCollider*)a->getComponent(ComponentType::SphereCollider);

        return 1;
    }

    //////////////////////////////////
    //          TRANSFORM           //
    //////////////////////////////////
    int ScriptEngine::s_transform_getpos(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.transform");
        luaL_argcheck(L, ud != NULL, 1, "`transform' expected");

        Transform *a = *(Transform**)ud;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *b = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *b = a->pos;

        return 1;
    }

    int ScriptEngine::s_transform_setpos(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.transform");
        luaL_argcheck(L, ud != NULL, 1, "`transform' expected");

        Transform *a = *(Transform**)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");

        m3d::vec3 *b = (m3d::vec3*)ud;

        a->pos = *b;

        return 0;
    }

    int ScriptEngine::s_transform_getrot(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.transform");
        luaL_argcheck(L, ud != NULL, 1, "`transform' expected");

        Transform *a = *(Transform**)ud;

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *b = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *b = a->rot;

        return 1;
    }

    int ScriptEngine::s_transform_setrot(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.transform");
        luaL_argcheck(L, ud != NULL, 1, "`transform' expected");

        Transform *a = *(Transform**)ud;

        ud = luaL_checkudata(L, 2, "Myth.quat");
        luaL_argcheck(L, ud != NULL, 2, "`quat' expected");

        m3d::quat *b = (m3d::quat*)ud;

        a->rot = *b;

        return 0;
    }

    ///////////////////////////////
    //          MATERIAL         //
    ///////////////////////////////

    int ScriptEngine::s_material_get(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        luaL_argcheck(L, lua_isstring(L, 1), 1, "`string' expected");

        std::string mat_name = lua_tostring(L, 1);

        size_t nbytes = sizeof(Material*);
        Material **b = (Material**)lua_newuserdata(L, nbytes);
        luaL_getmetatable(L, "Myth.material");
        lua_setmetatable(L, -2);

        *b = s_bound->m_resources->getMaterial(s_bound->m_resources->getIndex(mat_name));

        return 1;
    }

    int ScriptEngine::s_material_setuniformv3(lua_State *L)
    {
        if(!lua_argcount(L, 3)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.material");
        luaL_argcheck(L, ud != NULL, 1, "`material' expected");
        Material *a = *(Material**)ud;

        luaL_argcheck(L, lua_isstring(L, 2), 2, "'string' expected");
        const char *uniform_name = lua_tostring(L, 2);

        ud = luaL_checkudata(L, 3, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 3, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        a->setUniform(s_bound->m_resources->getShaderUniformLocation(a->getShader(), uniform_name), *b);

        return 0;
    }

    ///////////////////////////////
    //      SPHERE COLLIDER      //
    ///////////////////////////////

    int ScriptEngine::s_sphere_getPos(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.sphere");
        luaL_argcheck(L, ud != NULL, 1, "`sphere' expected");

        SphereCollider *a = *(SphereCollider**)ud;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *b = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *b = a->getOffset();

        return 1;
    }

    int ScriptEngine::s_sphere_getRadius(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.sphere");
        luaL_argcheck(L, ud != NULL, 1, "`sphere' expected");

        SphereCollider *a = *(SphereCollider**)ud;

        lua_pushnumber(L, a->getRadius());

        return 1;
    }

    int ScriptEngine::s_sphere_setPos(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.sphere");
        luaL_argcheck(L, ud != NULL, 1, "`sphere' expected");

        SphereCollider *a = *(SphereCollider**)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");

        m3d::vec3 *b = (m3d::vec3*)ud;

        a->setOffset(*b);

        return 0;
    }

    int ScriptEngine::s_sphere_setRadius(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.sphere");
        luaL_argcheck(L, ud != NULL, 1, "`sphere' expected");

        SphereCollider *a = *(SphereCollider**)ud;

        luaL_argcheck(L, lua_isnumber(L, 2), 2, "`number' expected");
        float b = lua_tonumber(L, 2);

        a->setRadius(b);

        return 0;
    }

    ///////////////////////////////
    //          INPUT            //
    ///////////////////////////////
    int ScriptEngine::s_input_getkey(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;
        luaL_argcheck(L, lua_isnumber(L, 1), 1, "`number' expected");

        unsigned key = lua_tointeger(L, 1);

        lua_pushboolean(L, s_bound->m_window->getInput().getKey((dgn::Key)key));
        return 1;
    }

    int ScriptEngine::s_input_getkeydown(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;
        luaL_argcheck(L, lua_isnumber(L, 1), 1, "`number' expected");

        unsigned key = lua_tointeger(L, 1);

        lua_pushboolean(L, s_bound->m_window->getInput().getKeyDown((dgn::Key)key));
        return 1;
    }

    int ScriptEngine::s_input_getkeyup(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;
        luaL_argcheck(L, lua_isnumber(L, 1), 1, "`number' expected");

        unsigned key = lua_tointeger(L, 1);

        lua_pushboolean(L, s_bound->m_window->getInput().getKeyUp((dgn::Key)key));
        return 1;
    }

    int ScriptEngine::s_input_controlleraxis(lua_State *L)
    {
        if(!lua_argcount(L, 3)) return 0;
        luaL_argcheck(L, lua_isnumber(L, 1), 1, "`number' expected");
        luaL_argcheck(L, lua_isnumber(L, 2), 2, "`number' expected");
        luaL_argcheck(L, lua_isnumber(L, 3), 3, "`number' expected");

        unsigned gamepad = lua_tointeger(L, 1);
        unsigned axis = lua_tointeger(L, 2);
        float deadzone = lua_tonumber(L, 3);

        lua_pushnumber(L, s_bound->m_window->getInput().getGamepadAxis(
                            gamepad,(dgn::GamepadAxis)axis, deadzone));
        return 1;
    }
}
