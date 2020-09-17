#include "../ScriptEngine.h"
#include "../luaEX.h"

namespace myth
{
    void ScriptEngine::input_openlibs()
    {
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
            luaL_setfuncs(L, inputNamespace, 0);

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

    int ScriptEngine::s_input_getkey(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;
        luaL_argcheck(L, lua_isnumber(L, 1), 1, "`number' expected");

        unsigned key = lua_tointeger(L, 1);

        lua_pushboolean(L, ScriptEngine::s_bound->m_window->getInput().getKey((dgn::Key)key));
        return 1;
    }

    int ScriptEngine::s_input_getkeydown(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;
        luaL_argcheck(L, lua_isnumber(L, 1), 1, "`number' expected");

        unsigned key = lua_tointeger(L, 1);

        lua_pushboolean(L, ScriptEngine::s_bound->m_window->getInput().getKeyDown((dgn::Key)key));
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

        lua_pushnumber(L, ScriptEngine::s_bound->m_window->getInput().getGamepadAxis(
                            gamepad,(dgn::GamepadAxis)axis, deadzone));
        return 1;
    }
}
