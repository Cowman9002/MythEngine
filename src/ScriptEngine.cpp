#include "ScriptEngine.h"

#include <m3d/m3d.h>
#include <DragonEngine/Camera.h>

void print_error(lua_State* L)
 {
  // The error message is on top of the stack.
  // Fetch it, print it and then pop it off the stack.
  const char* message = lua_tostring(L, -1);
  puts(message);
  lua_pop(L, 1);
}

namespace myth
{
    static ScriptEngine *s_bound;

    bool ScriptEngine::initialize(dgn::Window *window, RenderingEngine* render)
    {
        if(!window || !render) return false;

        m_window = window;
        m_render = render;

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
            {nullptr, nullptr}
        };

        luaL_Reg quatObject[] =
        {
            {"angleAxis", s_quat_angleaxis},
            {"getRight", s_quat_getright},
            {"getUp", s_quat_getup},
            {"getForward", s_quat_getforward},
            {"rotate", s_quat_rotate},
            {"mul", s_quat_mul},
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
        lua_setglobal(L, "camera");

        lua_newtable(L);
        {
            luaL_setfuncs(L, inputNamespace, 0);
        }
        lua_setglobal(L, "Input");

        s_bound = this;

        return true;
    }

    void ScriptEngine::terminate()
    {
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

        printf("%s created\n", instance);

        lua_setglobal(L, instance);
    }

    void ScriptEngine::callScript(const std::string& filename)
    {
        int result = luaL_loadfile(L, filename.c_str());
        if ( result != LUA_OK )
        {
            print_error(L);
            return;
        }
        result = lua_pcall(L, 0, LUA_MULTRET, 0);
        if ( result != LUA_OK )
        {
            print_error(L);
            return;
        }
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
            printf("Method %s no exist\n", method.c_str());
        }
    }

    ///////////////////////////////////////////
    //            SCRIPT FUNCTIONS           //
    ///////////////////////////////////////////

    bool lua_argcount(lua_State *L, unsigned exp)
    {
        unsigned got = lua_gettop(L);
        if(exp != got)
        {
            luaL_error(L, "Error: expected %I arguments, got %I", exp, got);
            return false;
        }
        return true;
    }

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

    /////////////////////////////////
    //          CAMERA             //
    /////////////////////////////////

    int ScriptEngine::s_camera_get(lua_State *L)
    {
        lua_pushlightuserdata(L, s_bound->m_render->getCamera());
        luaL_getmetatable(L, "Myth.camera");
        lua_setmetatable(L, -2);

        return 1;
    }

    int ScriptEngine::s_camera_setpos(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.camera");
        luaL_argcheck(L, ud != NULL, 1, "`camera' expected");

        dgn::Camera *a = (dgn::Camera *)ud;

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

        dgn::Camera *a = (dgn::Camera *)ud;

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

        dgn::Camera *a = (dgn::Camera *)ud;

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

        dgn::Camera *a = (dgn::Camera *)ud;

        size_t nbytes = sizeof(m3d::quat);
        m3d::quat *b = (m3d::quat*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.quat");
        lua_setmetatable(L, -2);

        *b = a->rotation;

        return 1;
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
