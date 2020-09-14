#pragma once

#include <lua/lua.hpp>
#include <string>
#include <vector>

#include <DragonEngine/Window.h>
#include "RenderingEngine.h"

namespace myth
{
    class Entity;
    class ScriptEngine
    {
    private:
        lua_State *L;
        dgn::Window *m_window;
        RenderingEngine *m_render;

        Entity *m_current_entity;

    public:
        bool initialize(dgn::Window *window, RenderingEngine *render);
        void terminate();

        inline void setCurrentEntity(Entity* e) { m_current_entity = e; };

        void createNewComponent(const unsigned& entity_id, const std::string& comp_name);

        void callScript(const std::string& filename);

        void callParameterlessMethod(const unsigned& entity_id, const std::string& comp_name, const std::string& method);
        void setNamespaceValue(const std::string& namesp, const std::string& valueName, const double& value);
    private:

        ////////////////////////////
        //          VEC3          //
        ////////////////////////////
        static int s_vec3_new(lua_State *L);
        static int s_vec3_getx(lua_State *L);
        static int s_vec3_gety(lua_State *L);
        static int s_vec3_getz(lua_State *L);
        static int s_vec3_setx(lua_State *L);
        static int s_vec3_sety(lua_State *L);
        static int s_vec3_setz(lua_State *L);
        static int s_vec3_add(lua_State *L);
        static int s_vec3_sub(lua_State *L);
        static int s_vec3_mul(lua_State *L);
        static int s_vec3_div(lua_State *L);
        static int s_vec3_down(lua_State *L);

        ///////////////////////////////////
        //          QUATERNION           //
        ///////////////////////////////////

        static int s_quat_angleaxis(lua_State *L);
        static int s_quat_getright(lua_State *L);
        static int s_quat_getup(lua_State *L);
        static int s_quat_getforward(lua_State *L);
        static int s_quat_rotate(lua_State *L);
        static int s_quat_mul(lua_State *L);

        /////////////////////////////////
        //            CAMERA           //
        /////////////////////////////////

        static int s_camera_get(lua_State *L);
        static int s_camera_setpos(lua_State *L);
        static int s_camera_setrot(lua_State *L);
        static int s_camera_getpos(lua_State *L);
        static int s_camera_getrot(lua_State *L);

        //////////////////////////////
        //          ENTITY          //
        //////////////////////////////
        static int s_entity_getself(lua_State *L);
        static int s_entity_gettransform(lua_State *L);

        //////////////////////////////////
        //          TRANSFORM           //
        //////////////////////////////////
        static int s_transform_getpos(lua_State *L);
        static int s_transform_setpos(lua_State *L);

        ///////////////////////////////
        //          INPUT            //
        ///////////////////////////////
        static int s_input_getkey(lua_State *L);
        static int s_input_getkeydown(lua_State *L);
        static int s_input_getkeyup(lua_State *L);
        static int s_input_controlleraxis(lua_State *L);
    };
}
