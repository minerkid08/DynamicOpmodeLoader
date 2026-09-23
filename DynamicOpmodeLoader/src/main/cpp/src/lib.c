#include "lib.h"
#include "lua/lauxlib.h"
#include "lua/lua.h"

#include <dirent.h>
#include <stdio.h>

static int ls(lua_State* l)
{
	if (lua_gettop(l) != 1)
		luaL_error(l, "expected one arg");
	if (lua_type(l, 1) != LUA_TSTRING)
		luaL_error(l, "expected string as arg 1");

	DIR* d;
	struct dirent* dir;

	d = opendir(lua_tostring(l, 1));

	if (d)
	{
		int n = 1;
		lua_newtable(l);
		while ((dir = readdir(d)) != NULL)
		{
			lua_newtable(l);
			lua_pushstring(l, dir->d_name);
			lua_setfield(l, -2, "name");
			lua_pushboolean(l, dir->d_type == DT_REG);
			lua_setfield(l, -2, "file");
			lua_seti(l, -2, n);
			n++;
		}
		closedir(d);
		return 1;
	}
	else
	{
		luaL_error(l, "failed to open dir");
		return 0;
	}
}

int buffer2dNew(lua_State* l);

void addFunc(lua_State* l)
{
	lua_getglobal(l, "io");
	lua_pushcfunction(l, ls);
	lua_setfield(l, -2, "list");
	lua_pop(l, 1);
	lua_pushcfunction(l, buffer2dNew);
	lua_setglobal(l, "newBuf");
}
