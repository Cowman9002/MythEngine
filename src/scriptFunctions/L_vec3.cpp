#include "LuaScripts.h"
#include "../luaEX.h"

#include <m3d/vec3.h>

namespace myth
{
    void l_vec3_openlibs(lua_State *L)
    {
        luaL_Reg vec3Object[] =
        {
            {"new", l_vec3_new},
            {"getx", l_vec3_getx},
            {"setx", l_vec3_setx},
            {"gety", l_vec3_gety},
            {"sety", l_vec3_sety},
            {"getz", l_vec3_getz},
            {"setz", l_vec3_setz},
            {"add", l_vec3_add},
            {"sub", l_vec3_sub},
            {"mul", l_vec3_mul},
            {"div", l_vec3_div},
            {"dot", l_vec3_dot},
            {"down", l_vec3_down},
            {"up", l_vec3_up},
            {"right", l_vec3_right},
            {"left", l_vec3_left},
            {"forward", l_vec3_forward},
            {"back", l_vec3_back},
            {"normalized", l_vec3_normalized},
            {"lerp", l_vec3_lerp},
            {"slerp", l_vec3_slerp},
            {"distSqr", l_vec3_distancesqr},
            {"dist", l_vec3_distance},
            {"reflection", l_vec3_reflection},
            {"equals", l_vec3_equals},
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
        lua_setglobal(L, "Vec3");
    }

    int l_vec3_new(lua_State *L)
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

    int l_vec3_getx(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");

        m3d::vec3 *a = (m3d::vec3 *)ud;

        lua_pushnumber(L, a->x);

        return 1;
    }

    int l_vec3_gety(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");

        m3d::vec3 *a = (m3d::vec3 *)ud;

        lua_pushnumber(L, a->y);

        return 1;
    }

    int l_vec3_getz(lua_State *L)
    {
        if(!lua_argcount(L, 1)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");

        m3d::vec3 *a = (m3d::vec3 *)ud;

        lua_pushnumber(L, a->z);

        return 1;
    }

    int l_vec3_setx(lua_State *L)
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

    int l_vec3_sety(lua_State *L)
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

    int l_vec3_setz(lua_State *L)
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

    int l_vec3_add(lua_State *L)
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

    int l_vec3_sub(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3 *)ud;

        m3d::vec3 c;

        if(lua_isnumber(L, 2)) // subtracting number
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

        *d = c;

        return 1;
    }
    int l_vec3_mul(lua_State *L)
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

    int l_vec3_div(lua_State *L)
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

    int l_vec3_dot(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        lua_pushnumber(L, a->dot(*b));

        return 1;
    }

    int l_vec3_down(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::down();

        return 1;
    }

    int l_vec3_up(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::up();

        return 1;
    }

    int l_vec3_left(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::left();

        return 1;
    }

    int l_vec3_right(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::right();

        return 1;
    }

    int l_vec3_forward(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::forwards();

        return 1;
    }

    int l_vec3_back(lua_State *L)
    {
        if(!lua_argcount(L, 0)) return 0;

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *a = (m3d::vec3*)lua_newuserdata(L, nbytes);

        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *a = m3d::vec3::back();

        return 1;
    }

    int l_vec3_normalized(lua_State *L)
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

    int l_vec3_lerp(lua_State *L)
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

    int l_vec3_slerp(lua_State *L)
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

    int l_vec3_distancesqr(lua_State *L)
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

    int l_vec3_distance(lua_State *L)
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

    int l_vec3_reflection(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        m3d::vec3 c = m3d::vec3::reflect(*a, *b);

        size_t nbytes = sizeof(m3d::vec3);
        m3d::vec3 *d = (m3d::vec3*)lua_newuserdata(L, nbytes);
        luaL_getmetatable(L, "Myth.vec3");
        lua_setmetatable(L, -2);

        *d = c;

        return 1;
    }

    int l_vec3_equals(lua_State *L)
    {
        if(!lua_argcount(L, 2)) return 0;

        void *ud = luaL_checkudata(L, 1, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 1, "`vec3' expected");
        m3d::vec3 *a = (m3d::vec3*)ud;

        ud = luaL_checkudata(L, 2, "Myth.vec3");
        luaL_argcheck(L, ud != NULL, 2, "`vec3' expected");
        m3d::vec3 *b = (m3d::vec3*)ud;

        lua_pushboolean(L, *a == *b);

        return 1;
    }
}
