#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"
#include <stdio.h>

static int traceback(lua_State* L)
{
	lua_getglobal(L, "debug");
	lua_getfield(L, -1, "traceback");
	lua_pushvalue(L, 1);
	lua_pushinteger(L, 2);
	lua_call(L, 2, 1);
	fprintf(stderr, "%s\n", lua_tostring(L, -1));
	return 1;
}

int main(int argc, char** argv)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_pushcfunction(L, traceback);
	int rv = luaL_loadfile(L, "main.lua");
	if (rv)
	{
		fprintf(stderr, "%s\n", lua_tostring(L, -1));
		return rv;
	}
	else
	{
		return lua_pcall(L, 0, 0, lua_gettop(L) - 1);
	}
}
