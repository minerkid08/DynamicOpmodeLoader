#include "functionBuilder.h"
#include "callback.h"
#include "dynList.h"
#include "function.h"
#include "global.h"
#include "jni.h"
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "type.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

#define pushTable Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_pushTable
#define popTable Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_popTable

int tableLevel = 0;

static char** tableNames;

void fbInit()
{
	initClassFunctions();
  initGlobalFunctions();
  initStaticFunctions();
	tableNames = dynList_new(0, sizeof(char*));
	dynList_reserve((void**)&tableNames, 5);
}

void fbInitLua()
{
	initClassFunctionsLua();
	initGlobalFunctionsLua();
  initStaticFunctionsLua();
}

void fbReset()
{
  resetClassFunctions();
  resetGlobalFunctions();
  resetStaticFunctions();
	int s = dynList_size(tableNames);
	for (int i = 0; i < s; i++)
		free(tableNames[i]);
	dynList_resize((void**)&tableNames, 0);
	tableLevel = 0;
}

JNIEXPORT void JNICALL pushTable(JNIEnv* env, jobject this, jstring name)
{
	lua_newtable(l);
	const char* c = (*env)->GetStringUTFChars(env, name, 0);
	char* n = malloc(strlen(c) + 1);
	strcpy(n, c);
	tableNames[tableLevel] = n;
	(*env)->ReleaseStringUTFChars(env, name, c);

	tableLevel++;

	dynList_resize((void**)&tableNames, tableLevel);
	print("pushed table %d, %s", tableLevel - 1, n);
}

JNIEXPORT void JNICALL popTable(JNIEnv* env, jobject this)
{
	char* name = tableNames[tableLevel - 1];

	dynList_resize((void**)&tableNames, tableLevel - 1);
	tableLevel--;

	if (tableLevel > 0)
		lua_setfield(l, -2, name);
	else
		lua_setglobal(l, name);
	print("poped table %d, %s", tableLevel, name);
	free(name);
}

jvalue* checkArgs(lua_State* l, Function* fun, int s)
{
	int argc = lua_gettop(l) - 1 - s;
	if (fun->argc != argc)
	{
		luaL_error(l, "expected %d args, got %d", fun->argc, argc);
	}

	jvalue* args = 0;
	if (argc > 0)
	{
		args = malloc(sizeof(jvalue) * argc);

		for (int i = 0; i < argc; i++)
		{
			char type = lua_type(l, i + 2 + s);
			char type2 = fun->argTypes[i];
			if (type2 == TINT || type2 == TFLOAT)
				type2 = LUA_TNUMBER;
            if (type2 == LUA_TNIL)
                type2 = LUA_TTABLE;

			if (type != type2)
			{
				const char* msg;
				const char* typearg;
				if (luaL_getmetafield(l, i + 2 + s, "__name") == LUA_TSTRING)
					typearg = lua_tostring(l, -1);
				else if (lua_type(l, i + 2 + s) == LUA_TLIGHTUSERDATA)
					typearg = "light userdata";
				else
					typearg = luaL_typename(l, i + 2 + s);
				msg = lua_pushfstring(l, "%s expected, got %s", lua_typename(l, type2), typearg);
				luaL_argerror(l, i + 1, msg);
				return 0;
			}
			switch (type)
			{
			case LUA_TNUMBER: {
				if (fun->argTypes[i] == LUA_TNUMBER)
					args[i].d = lua_tonumber(l, i + 2 + s);
				if (fun->argTypes[i] == TFLOAT)
					args[i].f = (float)lua_tonumber(l, i + 2 + s);
				if (fun->argTypes[i] == TINT)
					args[i].i = (int)lua_tonumber(l, i + 2 + s);
			}
			break;
			case LUA_TBOOLEAN:
				args[i].z = lua_toboolean(l, i + 2 + s);
				break;
			case LUA_TSTRING:
				const char* str = lua_tostring(l, i + 2 + s);
				args[i].l = (*env)->NewStringUTF(env, str);
				break;
			case LUA_TTABLE:
				lua_getfield(l, i + 2 + s, "ref");
				args[i].l = lua_touserdata(l, -1);
				lua_pop(l, 1);
				break;
			case LUA_TFUNCTION:
				args[i].l = makeCallback(i + 2 + s);
				break;
			}
		}
	}
	return args;
}

int call(lua_State* l, Function* fun, jobject obj, jvalue* args)
{
	switch (fun->rtnType)
	{
	case LUA_TNIL: {
		(*env)->CallVoidMethodA(env, obj, fun->funId, args);
		free(args);
		return 0;
	}
	case TBUILDER: {
		(*env)->CallVoidMethodA(env, obj, fun->funId, args);
		free(args);
		lua_pushvalue(l, 2);
		return 1;
	}
	case LUA_TNUMBER: {
		double rtn = (*env)->CallDoubleMethodA(env, obj, fun->funId, args);
		lua_pushnumber(l, rtn);
		free(args);
		return 1;
	}
	case TFLOAT: {
		float rtn = (*env)->CallFloatMethodA(env, obj, fun->funId, args);
		lua_pushnumber(l, rtn);
		free(args);
		return 1;
	}
	case TINT: {
		int rtn = (*env)->CallIntMethodA(env, obj, fun->funId, args);
		lua_pushnumber(l, rtn);
		free(args);
		return 1;
	}
	case LUA_TBOOLEAN: {
		char rtn = (*env)->CallBooleanMethodA(env, obj, fun->funId, args);
		lua_pushboolean(l, rtn);
		free(args);
		return 1;
	}
	case LUA_TSTRING: {
		jstring rtn = (*env)->CallObjectMethodA(env, obj, fun->funId, args);
		const char* str = (*env)->GetStringUTFChars(env, rtn, NULL);
		lua_pushstring(l, str);
		(*env)->ReleaseStringUTFChars(env, rtn, str);
		free(args);
		return 1;
	}
	case LUA_TTABLE: {
		jobject res = (*env)->CallObjectMethodA(env, obj, fun->funId, args);
		jstring str = getClassName((*env)->GetObjectClass(env, res));
		const char* s = (*env)->GetStringUTFChars(env, str, NULL);
		free(args);

		lua_getglobal(l, s);
		int objPos = lua_gettop(l);
		if (lua_type(l, -1) != LUA_TTABLE)
			luaL_error(l, "attempted to return object of an unknown type \'%s\'", s);

		jobject ref = (*env)->NewGlobalRef(env, res);

		lua_newtable(l);
		lua_pushnil(l);
		while (lua_next(l, objPos) != 0)
		{
			lua_pushvalue(l, -2);
			lua_insert(l, -2);
			lua_settable(l, -4);
		}
		lua_pushlightuserdata(l, ref);
		lua_setfield(l, -2, "ref");

		luaL_getmetatable(l, "jobject");
		lua_setmetatable(l, -2);
		return 1;
	}
	}
	return 0;
}

int callStatic(lua_State* l, Function* fun, jclass obj, jvalue* args)
{
	switch (fun->rtnType)
	{
	case LUA_TNIL: {
		(*env)->CallStaticVoidMethodA(env, obj, fun->funId, args);
		free(args);
		return 0;
	}
	case TBUILDER: {
		(*env)->CallStaticVoidMethodA(env, obj, fun->funId, args);
		free(args);
		lua_pushvalue(l, 2);
		return 1;
	}
	case LUA_TNUMBER: {
		double rtn = (*env)->CallStaticDoubleMethodA(env, obj, fun->funId, args);
		lua_pushnumber(l, rtn);
		free(args);
		return 1;
	}
	case TFLOAT: {
		float rtn = (*env)->CallStaticFloatMethodA(env, obj, fun->funId, args);
		lua_pushnumber(l, rtn);
		free(args);
		return 1;
	}
	case TINT: {
		int rtn = (*env)->CallStaticIntMethodA(env, obj, fun->funId, args);
		lua_pushnumber(l, rtn);
		free(args);
		return 1;
	}
	case LUA_TBOOLEAN: {
		char rtn = (*env)->CallStaticBooleanMethodA(env, obj, fun->funId, args);
		lua_pushboolean(l, rtn);
		free(args);
		return 1;
	}
	case LUA_TSTRING: {
		jstring rtn = (*env)->CallStaticObjectMethodA(env, obj, fun->funId, args);
		const char* str = (*env)->GetStringUTFChars(env, rtn, NULL);
		lua_pushstring(l, str);
		(*env)->ReleaseStringUTFChars(env, rtn, str);
		free(args);
		return 1;
	}
	case LUA_TTABLE: {
		jobject res = (*env)->CallObjectMethodA(env, obj, fun->funId, args);
		jstring str = getClassName((*env)->GetObjectClass(env, res));
		const char* s = (*env)->GetStringUTFChars(env, str, NULL);
		free(args);

		lua_getglobal(l, s);
		int objPos = lua_gettop(l);
		if (lua_type(l, -1) != LUA_TTABLE)
			luaL_error(l, "attempted to return object of an unknown type \'%s\'", s);

		jobject ref = (*env)->NewGlobalRef(env, res);

		lua_newtable(l);
		lua_pushnil(l);
		while (lua_next(l, objPos) != 0)
		{
			lua_pushvalue(l, -2);
			lua_insert(l, -2);
			lua_settable(l, -4);
		}
		lua_pushlightuserdata(l, ref);
		lua_setfield(l, -2, "ref");

		luaL_getmetatable(l, "jobject");
		lua_setmetatable(l, -2);
		return 1;
	}
	}
	return 0;
}

