#include "../ScriptEngine.h"
#include "../luaEX.h"

namespace myth
{
    void ScriptEngine::camera_openlibs()
    {
        luaL_Reg cameraObject[] =
        {
            {"get", s_camera_get},
            {"setPos", s_camera_setpos},
            {"setRot", s_camera_setrot},
            {"getPos", s_camera_getpos},
            {"getRot", s_camera_getrot},
            {nullptr, nullptr}
        };

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.camera");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, cameraObject, 0);
        }
        lua_setglobal(L, "Camera");
    }

    int ScriptEngine::s_camera_get(lua_State *L)
    {
        size_t nbytes = sizeof(dgn::Camera*);
        dgn::Camera **d = (dgn::Camera**)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.camera");
        lua_setmetatable(L, -2);

        *d = ScriptEngine::s_bound->m_render->getCamera();

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
}
