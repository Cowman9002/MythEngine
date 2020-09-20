#pragma once

#include <lua/lua.hpp>
#include <string>
#include <vector>

#include <DragonEngine/Window.h>
#include "RenderingEngine.h"
#include "ResourceManager.h"

namespace myth
{
    class Entity;
    class ScriptEngine
    {
    private:
        lua_State *L;
        dgn::Window *m_window;
        RenderingEngine *m_render;
        ResourceManager *m_resources;

        Entity *m_current_entity;

        static ScriptEngine *s_bound;

    public:
        bool initialize(dgn::Window *window, RenderingEngine *render, ResourceManager *resources);
        void terminate();

        inline void setCurrentEntity(Entity* e) { m_current_entity = e; };

        void createNewComponent(const unsigned& entity_id, const std::string& comp_name);

        void callScript(const std::string& filename);

        void callParameterlessMethod(const unsigned& entity_id, const std::string& comp_name, const std::string& method);
        void setNamespaceValue(const std::string& namesp, const std::string& valueName, const double& value);
    private:

        ///////////////////////////////
        //          MATERIAL         //
        ///////////////////////////////

        void material_openlibs();
        static int s_material_get(lua_State *L);
        static int s_material_setuniformv3(lua_State *L);

        /////////////////////////////////
        //            CAMERA           //
        /////////////////////////////////

        void camera_openlibs();
        static int s_camera_get(lua_State *L);
        static int s_camera_setpos(lua_State *L);
        static int s_camera_setrot(lua_State *L);
        static int s_camera_getpos(lua_State *L);
        static int s_camera_getrot(lua_State *L);

        //////////////////////////////
        //          ENTITY          //
        //////////////////////////////

        void entity_openlibs();
        static int s_entity_getself(lua_State *L);
        static int s_entity_gettransform(lua_State *L);
        static int s_entity_getrigidbody(lua_State *L);
        static int s_entity_getsphere(lua_State *L);
        static int s_entity_getaabb(lua_State *L);

        ///////////////////////////////
        //      SPHERE COLLIDER      //
        ///////////////////////////////

        void sphere_openlibs();
        static int s_sphere_getpos(lua_State *L);
        static int s_sphere_getradius(lua_State *L);
        static int s_sphere_setpos(lua_State *L);
        static int s_sphere_setradius(lua_State *L);

        //////////////////////////////
        //       AABB COLLIDER      //
        //////////////////////////////

        void aabb_openlibs();
        static int s_aabb_getpos(lua_State *L);
        static int s_aabb_getextents(lua_State *L);
        static int s_aabb_setpos(lua_State *L);
        static int s_aabb_setextents(lua_State *L);

        ///////////////////////////////
        //          INPUT            //
        ///////////////////////////////

        void input_openlibs();
        static int s_input_getkey(lua_State *L);
        static int s_input_getkeydown(lua_State *L);
        static int s_input_getkeyup(lua_State *L);
        static int s_input_controlleraxis(lua_State *L);
        static int s_input_getcontrollerbutton(lua_State *L);
    };
}
