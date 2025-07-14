#include "functionBuilder.h"
#include "jni_md.h"
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
#define close Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_close
#define loadOpmode Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_loadOpmode
#define start Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_start
#define update Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_update
#define callFun Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_callFun
#define callOpmodeFun Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_callOpmodeFun

int currentOpmode;
lua_State* l;
JNIEnv* env;
Opmode* opmodes;
Function* functions;
jobject* objects;
jobject currentObject;

int addOpmode(lua_State* l)
{
	int top = lua_gettop(l);
	if (top != 1)
		luaL_error(l, "expected 1 argument, got %d", top);
	if (lua_type(l, -1) != LUA_TTABLE)
		luaL_typeerror(l, 1, "table");

	int opmodeId = dynList_size(opmodes);
	dynList_resize((void**)&opmodes, opmodeId + 1);
	Opmode* opmode = opmodes + opmodeId;
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
	lua_setglobal(l, "data");

	return 0;
}

JNIEXPORT int JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	opmodes = dynList_new(0, sizeof(Opmode));
	dynList_reserve((void**)&opmodes, 5);
	
  fbInit();
  return JNI_VERSION_20;
}

JNIEXPORT void JNICALL close(JNIEnv* env2, jobject this)
{
	env = env2;
	int opmodeLen = dynList_size(opmodes);
	for (int i = 0; i < opmodeLen; i++)
	{
		char* str = opmodes[i].name;
		free(str);
	}
	dynList_resize((void**)&opmodes, 0);
	fbReset();
	int status = lua_status(l);
	if (status != LUA_OK)
		print("lua not ok");
	lua_close(l);
	print("lua_state closed");
}

char inited = 0;
JNIEXPORT void JNICALL init2(JNIEnv* env2, jobject this, jobject stdlib)
{
	env = env2;
	initUtils(stdlib);

	print("lua_state initalising");

	l = luaL_newstate();

	luaL_openlibs(l);

#ifndef ANDROID
	if (luaL_dostring(l, "package.path = \"./lua/?.lua\""))
#else
	if (luaL_dostring(l, "package.path = \"/sdcard/lua/?.lua\""))
#endif
	{
		err("%s", lua_tostring(l, -1));
		return;
	}
	lua_pushcfunction(l, addOpmode);
	lua_setglobal(l, "addOpmode");

	lua_newtable(l);
	lua_setglobal(l, "data");

	fbInitLua();
	print("lua_state initalised");
}

JNIEXPORT jobjectArray JNICALL init(JNIEnv* env2, jobject this)
{
	env = env2;
#ifndef ANDROID
	if (luaL_dofile(l, "lua/init.lua"))
#else
	if (luaL_dofile(l, "/sdcard/lua/init.lua"))
#endif
	{
		err("%s", lua_tostring(l, -1));
		return NULL;
	}

	int opmodeCount = dynList_size(opmodes);

	jobjectArray arr = (*env)->NewObjectArray(env, opmodeCount, (*env)->FindClass(env, "java/lang/String"), NULL);

	print("loaded %d opmodes\n", opmodeCount);

	for (int i = 0; i < opmodeCount; i++)
	{
		Opmode* opmode = opmodes + i;
		jstring str = (*env)->NewStringUTF(env, opmode->name);
		(*env)->SetObjectArrayElement(env, arr, i, str);
		(*env)->DeleteLocalRef(env, str);
	}
	return arr;
}

JNIEXPORT void JNICALL loadOpmode(JNIEnv* env2, jobject this, jstring opmodeName)
{
	env = env2;
	currentOpmode = -1;
	const char* name = (*env)->GetStringUTFChars(env, opmodeName, NULL);
	int opmodeCount = dynList_size(opmodes);
	for (int i = 0; i < opmodeCount; i++)
	{
		Opmode* opmode = opmodes + i;
		if (strcmp(opmode->name, name) == 0)
		{
			currentOpmode = i;
			break;
		}
	}

	if (currentOpmode == -1)
	{
		err("cant find opmode \'%s\'", name);
		return;
	}

	lua_settop(l, 0);
	lua_getglobal(l, "data");
	lua_geti(l, -1, currentOpmode);
	lua_getfield(l, -1, "init");
	if (lua_type(l, -1) == LUA_TFUNCTION)
	{
		if (lua_pcall(l, 0, 0, 0))
		{
			err("%s", lua_tostring(l, -1));
			return;
		}
	}
	lua_settop(l, 2);
	print("loaded opmode \'%s\'\n", name);
}

JNIEXPORT void JNICALL start(JNIEnv* env2, jobject this, int recognitionId)
{
	env = env2;
	lua_getfield(l, -1, "start");
	if (lua_type(l, -1) == LUA_TFUNCTION)
	{
		lua_pushnumber(l, recognitionId);
		if (lua_pcall(l, 1, 0, 0))
		{
			err("%s", lua_tostring(l, -1));
		}
	}
	lua_settop(l, 2);
}

JNIEXPORT void JNICALL update(JNIEnv* env2, jobject this, double deltaTime, double elapsedTime)
{
	env = env2;
	lua_getfield(l, -1, "update");
	if (lua_type(l, -1) == LUA_TFUNCTION)
	{
		lua_pushnumber(l, deltaTime);
		lua_pushnumber(l, elapsedTime);
		if (lua_pcall(l, 2, 0, 0))
		{
			err("%s", lua_tostring(l, -1));
		}
	}
	lua_settop(l, 2);
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
			lua_pushboolean(l, value);
		}
		else if (strcmp(name, "Integer") == 0)
		{
			if (!getInt)
				getInt = (*env)->GetMethodID(env, class, "intValue", "()I");
			int value = (*env)->CallBooleanMethod(env, elem, getInt);
			lua_pushinteger(l, value);
		}
		else if (strcmp(name, "Double") == 0)
		{
			if (!getDouble)
				getDouble = (*env)->GetMethodID(env, class, "doubleValue", "()D");
			double value = (*env)->CallDoubleMethod(env, elem, getDouble);
			lua_pushnumber(l, value);
		}
		else if (strcmp(name, "String") == 0)
		{
			const char* str = (*env)->GetStringUTFChars(env, elem, 0);
			lua_pushstring(l, str);
			(*env)->ReleaseStringUTFChars(env, elem, str);
		}
		(*env)->ReleaseStringUTFChars(env, className, name);
	}
}

JNIEXPORT void JNICALL callFun(JNIEnv* env2, jobject this, jstring name, jobjectArray args)
{
	env = env2;
	const char* nameStr = (*env)->GetStringUTFChars(env, name, 0);
	lua_getglobal(l, nameStr);
	if (lua_type(l, -1) == LUA_TFUNCTION)
	{
		int len = (*env)->GetArrayLength(env, args);
		pushArgs(env, args, len);
		if (lua_pcall(l, len, 0, 0))
			err("%s", lua_tostring(l, -1));
	}
	else
		err("undefined function: \'%s\'", nameStr);
	lua_settop(l, 2);
}

JNIEXPORT void JNICALL callOpmodeFun(JNIEnv* env2, jobject this, jstring name, jobjectArray args)
{
	env = env2;
	const char* nameStr = (*env)->GetStringUTFChars(env, name, 0);
	lua_getfield(l, -1, nameStr);
	if (lua_type(l, -1) == LUA_TFUNCTION)
	{
		int len = (*env)->GetArrayLength(env, args);
		pushArgs(env, args, len);
		if (lua_pcall(l, len, 0, 0))
			err("%s", lua_tostring(l, -1));
	}
	else
		err("undefined opmode function: \'%s\'", nameStr);
	lua_settop(l, 2);
}
