#include "lua/lauxlib.h"
#include "lua/lua.h"
#include <stdlib.h>
#include <string.h>

static int writeStr(lua_State* l)
{
	if(lua_gettop(l) != 4)
		luaL_error(l, "expected 4 args, got %d", lua_gettop(l)); 

	int x = lua_tointeger(l, 2);
	int y = lua_tointeger(l, 3);
	const char* str = lua_tostring(l, 4);

	lua_getfield(l, 1, "data");
	void* data = lua_touserdata(l, -1);
	lua_pop(l, 1);
	lua_getfield(l, 1, "sizeX");
	int sizeX = lua_tointeger(l, -1);
	lua_pop(l, 1);

	int len = strlen(str);
	char* start = data + x + y * (sizeX + 1);
	memcpy(start, str, len);

	return 0;
}

static int readLine(lua_State* l)
{
	if(lua_gettop(l) != 2)
		luaL_error(l, "expected 2 args, got %d", lua_gettop(l)); 

	int y = lua_tointeger(l, 2);

	lua_getfield(l, 1, "data");
	void* data = lua_touserdata(l, -1);
	lua_pop(l, 1);
	lua_getfield(l, 1, "sizeX");
	int sizeX = lua_tointeger(l, -1);
	lua_pop(l, 1);
	char* start = data +  y * (sizeX + 1);
	start[sizeX] = 0;
	lua_pushstring(l, start);
	return 1;
}

static int bufferFill(lua_State* l)
{
	if(lua_gettop(l) != 2)
		luaL_error(l, "expected 2 arg, got %d", lua_gettop(l)); 

	const char* str = lua_tostring(l, 2); 

	lua_getfield(l, 1, "data");
	void* data = lua_touserdata(l, -1);
	lua_pop(l, 1);
	lua_getfield(l, 1, "sizeX");
	int sizeX = lua_tointeger(l, -1);
	lua_pop(l, 1);
	lua_getfield(l, 1, "sizeY");
	int sizeY = lua_tointeger(l, -1);
	lua_pop(l, 1);

	memset(data, str[0], (sizeX + 1) * sizeY);
	return 0;
}

static int bufferFree(lua_State* l)
{
	if(lua_gettop(l) != 1)
		luaL_error(l, "expected 1 arg, got %d", lua_gettop(l)); 

	lua_getfield(l, 1, "data");
	void* data = lua_touserdata(l, -1);
	lua_pop(l, 1);
	free(data);
	return 0;
}

int buffer2dNew(lua_State* l)
{
	if(lua_gettop(l) != 2)
		luaL_error(l, "expected two args, got %d", lua_gettop(l)); 

	int sizeX = lua_tointeger(l, 1);
	int sizeY = lua_tointeger(l, 2);

	void* data = malloc((sizeX + 1) * sizeY);

	lua_newtable(l);
	lua_pushinteger(l, sizeX);
	lua_setfield(l, -2, "sizeX");
	lua_pushinteger(l, sizeY);
	lua_setfield(l, -2, "sizeY");
	lua_pushlightuserdata(l, data);
	lua_setfield(l, -2, "data");

	lua_pushcfunction(l, writeStr);
	lua_setfield(l, -2, "write");
	lua_pushcfunction(l, readLine);
	lua_setfield(l, -2, "readLine");
	lua_pushcfunction(l, bufferFill);
	lua_setfield(l, -2, "fill");
	lua_pushcfunction(l, bufferFree);
	lua_setfield(l, -2, "free");

	return 1;
}
