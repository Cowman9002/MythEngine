#include "../ScriptEngine.h"
#include "../luaEX.h"

namespace myth
{
    void ScriptEngine::material_openlibs()
    {
        luaL_Reg materialObject[] =
        {
            {"get", s_material_get},
            {"uniformv3", s_material_setuniformv3},
            {nullptr, nullptr}
        };

        lua_newtable(L);
        {
            luaL_newmetatable(L, "Myth.material");

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);  /* pushes the metatable */
            lua_settable(L, -3);  /* metatable.__index = metatable */

            luaL_setfuncs(L, materialObject, 0);
        }
        lua_setglobal(L, "Material");
    }

    int ScriptEngine::s_material_get(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        luaL_argcheck(L, lua_isstring(L, 1), 1, "`string' expected");

        std::string mat_name = lua_tostring(L, 1);

        size_t nbytes = sizeof(Material*);
        Material **b = (Material**)lua_newuserdata(L, nbytes);
        luaL_getmetatable(L, "Myth.material");
        lua_setmetatable(L, -2);

        *b = ScriptEngine::s_bound->m_resources->getMaterial(ScriptEngine::s_bound->m_resources->getIndex(mat_name));

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

        a->setUniform(ScriptEngine::s_bound->m_resources->getShaderUniformLocation(a->getShader(), uniform_name), *b);

        return 0;
    }
}
