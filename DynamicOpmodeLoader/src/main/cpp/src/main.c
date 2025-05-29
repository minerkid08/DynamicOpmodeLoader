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
#define init2 Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_internalInit2
#define loadOpmode Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_loadOpmode
#define start Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_start
#define update Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_update
#define callFun Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_callFun
#define callOpmodeFun Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_callOpmodeFun

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

void internalInit()
{
	global.opmodes = dynList_new(0, sizeof(Opmode));
	dynList_reserve((void**)&global.opmodes, 5);
}

int inited = 0;

JNIEXPORT void JNICALL init2(JNIEnv* env, jobject this, jobject stdlib)
{
	global.env = env;
	initUtils(stdlib);
	global.opmodes = dynList_new(sizeof(Opmode), 5);
	dynList_resize((void*)&global.opmodes, 0);

	if (!inited)
	{
		fbInit();
		internalInit();
		inited = 1;
	}
	else
	{
		int opmodeLen = dynList_size(global.opmodes);
		for (int i = 0; i < opmodeLen; i++)
		{
			char* str = global.opmodes[i].name;
			free(str);
		}
		dynList_resize((void**)&global.opmodes, 0);
		fbReset();
		lua_close(global.l);
	}

	global.l = luaL_newstate();

	luaL_openlibs(global.l);

#ifndef ANDROID
	if (luaL_dostring(global.l, "package.path = \"./lua/?.lua\""))
#else
	if (luaL_dostring(global.l, "package.path = \"/sdcard/lua/?.lua\""))
#endif
	{
		err("%s", lua_tostring(global.l, -1));
		return;
	}
	lua_pushcfunction(global.l, addOpmode);
	lua_setglobal(global.l, "addOpmode");

	lua_newtable(global.l);
	lua_setglobal(global.l, "data");

	fbInitLua();
}

JNIEXPORT jobjectArray JNICALL init(JNIEnv* env, jobject this)
{
	global.env = env;
#ifndef ANDROID
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
			return;
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

static jmethodID getBool = 0L;
static jmethodID getInt = 0L;
static jmethodID getDouble = 0L;

void pushArgs(JNIEnv* env, jobjectArray args, int len)
{
	for (int i = 0; i < len; i++)
	{
		jobject elem = (*env)->GetObjectArrayElement(env, args, i);
		jclass class = (*env)->GetObjectClass(env, elem);
		jstring className = getClassName(class);
		const char* name = (*env)->GetStringUTFChars(env, className, 0);

		if (strcmp(name, "Boolean") == 0)
		{
			if (!getBool)
				getBool = (*env)->GetMethodID(env, class, "booleanValue", "()Z");
			char value = (*env)->CallBooleanMethod(env, elem, getBool);
			lua_pushboolean(global.l, value);
		}
		else if (strcmp(name, "Integer") == 0)
		{
			if (!getInt)
				getInt = (*env)->GetMethodID(env, class, "intValue", "()I");
			int value = (*env)->CallBooleanMethod(env, elem, getInt);
			lua_pushinteger(global.l, value);
		}
		else if (strcmp(name, "Double") == 0)
		{
			if (!getDouble)
				getDouble = (*env)->GetMethodID(env, class, "doubleValue", "()D");
			double value = (*env)->CallDoubleMethod(env, elem, getDouble);
			lua_pushnumber(global.l, value);
		}
		else if (strcmp(name, "String") == 0)
		{
			const char* str = (*env)->GetStringUTFChars(env, elem, 0);
			lua_pushstring(global.l, str);
			(*env)->ReleaseStringUTFChars(env, elem, str);
		}
		(*env)->ReleaseStringUTFChars(env, className, name);
	}
}

JNIEXPORT void JNICALL callFun(JNIEnv* env, jobject this, jstring name, jobjectArray args)
{
	global.env = env;
	const char* nameStr = (*env)->GetStringUTFChars(env, name, 0);
	lua_getglobal(global.l, nameStr);
	if (lua_type(global.l, -1) == LUA_TFUNCTION)
	{
		int len = (*env)->GetArrayLength(env, args);
		pushArgs(env, args, len);
		if (lua_pcall(global.l, len, 0, 0))
			err("%s", lua_tostring(global.l, -1));
	}
	else
		err("undefined function: \'%s\'", nameStr);
	lua_settop(global.l, 2);
}

JNIEXPORT void JNICALL callOpmodeFun(JNIEnv* env, jobject this, jstring name, jobjectArray args)
{
	global.env = env;
	const char* nameStr = (*env)->GetStringUTFChars(env, name, 0);
	lua_getfield(global.l, -1, nameStr);
	if (lua_type(global.l, -1) == LUA_TFUNCTION)
	{
		int len = (*env)->GetArrayLength(env, args);
		pushArgs(env, args, len);
		if (lua_pcall(global.l, len, 0, 0))
			err("%s", lua_tostring(global.l, -1));
	}
	else
		err("undefined opmode function: \'%s\'", nameStr);
	lua_settop(global.l, 2);
}
