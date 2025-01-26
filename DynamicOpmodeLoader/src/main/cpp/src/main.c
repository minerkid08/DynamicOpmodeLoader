#include "functionBuilder.h"
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynList.h"
#include "global.h"
#include "utils.h"

#define init Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_internalInit
#define loadOpmode Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_loadOpmode
#define start Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_start
#define update Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_update

Global global;

int addOpmode(lua_State* l)
{
	int top = lua_gettop(l);
	if (top != 1)
		luaL_error(l, "expected 1 argument, got %d", top);
	if (lua_type(l, -1) != LUA_TTABLE)
		luaL_typeerror(l, 1, "table");

	int opmodeId = dynList_size(global.opmodes);
	dynList_resize((void**)&global.opmodes, opmodeId + 1);
	Opmode* opmode = global.opmodes + opmodeId;
	opmode->id = opmodeId;

	lua_getfield(l, -1, "name");
	const char* name = lua_tostring(l, -1);
	int stringLen = strlen(name);

	opmode->name = malloc(stringLen + 1);
	strcpy(opmode->name, name);
	opmode->name[stringLen] = 0;

	lua_pop(l, 1);

	lua_getglobal(l, "data");
	lua_pushvalue(l, -2);
	lua_seti(l, -2, opmodeId);
	// lua_setglobal(l, "data");

	return 0;
}

int inited = 0;

JNIEXPORT jobjectArray JNICALL init(JNIEnv* env, jobject this, jobject stdlib)
{
	global.env = env;
	initUtils(stdlib);
	global.opmodes = dynList_new(sizeof(Opmode), 5);
	dynList_resize((void*)&global.opmodes, 0);

	if (inited)
	{
		reset();
		lua_close(global.l);
	}
	inited = 1;

	global.l = luaL_newstate();
	
	luaL_openlibs(global.l);
	
#ifdef _WIN64
	if(luaL_dostring(global.l, "package.path = \"./lua/?.lua\""))
#else
	if(luaL_dostring(global.l, "2package.path = \"/sdcard/lua/?.lua\""))
#endif
	{
		err("%s", lua_tostring(global.l, -1));
		return NULL;
	}
	lua_pushcfunction(global.l, addOpmode);
	lua_setglobal(global.l, "addOpmode");

	lua_newtable(global.l);
	lua_setglobal(global.l, "data");

	initFunctionBuilder();
#ifdef _WIN64
	if (luaL_dofile(global.l, "lua/init.lua"))
#else
	if (luaL_dofile(global.l, "/sdcard/lua/init.lua"))
#endif
	{
		err("%s", lua_tostring(global.l, -1));
		return NULL;
	}
	int opmodeCount = dynList_size(global.opmodes);

	jobjectArray arr = (*env)->NewObjectArray(env, opmodeCount, (*env)->FindClass(env, "java/lang/String"), NULL);

	print("loaded %d opmodes\n", opmodeCount);
	fflush(stdout);

	for (int i = 0; i < opmodeCount; i++)
	{
		Opmode* opmode = global.opmodes + i;
		jstring str = (*env)->NewStringUTF(env, opmode->name);
		(*env)->SetObjectArrayElement(env, arr, i, str);
		(*env)->DeleteLocalRef(env, str);
	}
	return arr;
}

JNIEXPORT void JNICALL loadOpmode(JNIEnv* env, jobject this, jstring opmodeName)
{
	global.env = env;
	global.currentOpmode = -1;
	const char* name = (*env)->GetStringUTFChars(env, opmodeName, NULL);
	int opmodeCount = dynList_size(global.opmodes);
	for (int i = 0; i < opmodeCount; i++)
	{
		Opmode* opmode = global.opmodes + i;
		if (strcmp(opmode->name, name) == 0)
		{
			global.currentOpmode = i;
			break;
		}
	}

	if (global.currentOpmode == -1)
	{
		err("cant find opmode \'%s\'", name);
		return;
	}

	lua_settop(global.l, 0);
	lua_getglobal(global.l, "data");
	lua_geti(global.l, -1, global.currentOpmode);
	lua_getfield(global.l, -1, "init");
	if (lua_type(global.l, -1) == LUA_TFUNCTION)
	{
		if (lua_pcall(global.l, 0, 0, 0))
		{
			err("%s", lua_tostring(global.l, -1));
		}
	}
	lua_settop(global.l, 2);
	print("loaded opmode \'%s\'\n", name);
}

JNIEXPORT void JNICALL start(JNIEnv* env, jobject this, int recognitionId)
{
	global.env = env;
	lua_getfield(global.l, -1, "start");
	if (lua_type(global.l, -1) == LUA_TFUNCTION)
	{
		lua_pushnumber(global.l, recognitionId);
		if (lua_pcall(global.l, 1, 0, 0))
		{
			err("%s", lua_tostring(global.l, -1));
		}
	}
	lua_settop(global.l, 2);
}

JNIEXPORT void JNICALL update(JNIEnv* env, jobject this, double deltaTime, double elapsedTime)
{
	global.env = env;
	lua_getfield(global.l, -1, "update");
	if (lua_type(global.l, -1) == LUA_TFUNCTION)
	{
		lua_pushnumber(global.l, deltaTime);
		lua_pushnumber(global.l, elapsedTime);
		if (lua_pcall(global.l, 2, 0, 0))
		{
			err("%s", lua_tostring(global.l, -1));
		}
	}
	lua_settop(global.l, 2);
}
