#include "functionBuilder.h"
#include "dynList.h"
#include "function.h"
#include "global.h"
#include "jni.h"
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define addObject Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_setCurrentObject
#define createClass Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_createClass
#define addFun Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_addFunction
#define addFunc Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_addFunctionc

jmethodID getClassFun = 0l;

jstring getClassName(jclass class)
{
	if (!getClassFun)
	{
		jclass c = (*global.env)->GetObjectClass(global.env, class);
		getClassFun = (*global.env)->GetMethodID(global.env, c, "getSimpleName", "()Ljava/lang/String;");
	}
	return (*global.env)->CallObjectMethod(global.env, class, getClassFun);
}

int callFunc(lua_State* l);
int callFunc2(lua_State* l);

void fbInit()
{
	global.objects = dynList_new(0, sizeof(jobject));
	dynList_reserve((void**)&global.objects, 10);

	global.functions = dynList_new(0, sizeof(Function));
	dynList_reserve((void**)&global.functions, 20);
}

void fbInitLua()
{
	luaL_newmetatable(global.l, "luaFunc");
	lua_pushcfunction(global.l, callFunc);
	lua_setfield(global.l, -2, "__call");

	luaL_newmetatable(global.l, "luaFunc2");
	lua_pushcfunction(global.l, callFunc2);
	lua_setfield(global.l, -2, "__call");
}

void fbReset()
{
	int s = dynList_size(global.objects);
	for (int i = 0; i < s; i++)
		(*global.env)->DeleteGlobalRef(global.env, global.objects[i]);
	dynList_resize((void**)&global.objects, 0);
	dynList_resize((void**)&global.functions, 0);
}

JNIEXPORT void JNICALL createClass(JNIEnv* env, jobject this, jstring str)
{
	const char* s = (*env)->GetStringUTFChars(env, str, NULL);
	lua_newtable(global.l);
	lua_setglobal(global.l, s);
	(*env)->ReleaseStringUTFChars(env, str, s);
}

JNIEXPORT void JNICALL addObject(JNIEnv* env, jobject this, jobject object)
{
	global.env = env;

	jobject obj = (*env)->NewGlobalRef(env, object);
	global.currentObject = obj;

	int objectId = dynList_size(global.objects);
	dynList_resize((void**)&global.objects, objectId + 1);
	*(global.objects + objectId) = obj;
}

JNIEXPORT void JNICALL addFun(JNIEnv* env, jobject this, jstring name, jstring signature, int rtnType, int argc)
{
	const char* name2 = (*env)->GetStringUTFChars(env, name, NULL);
	const char* signature2 = (*env)->GetStringUTFChars(env, signature, NULL);

	global.env = env;

	unsigned long long functionId = dynList_size(global.functions);
	dynList_resize((void**)&global.functions, functionId + 1);
	Function* fun = global.functions + functionId;

	fflush(stdout);

	function_init(fun, name2, signature2, rtnType, argc);

	lua_newtable(global.l);
	lua_pushinteger(global.l, functionId);
	lua_setfield(global.l, -2, "id");
	luaL_getmetatable(global.l, "luaFunc");
	lua_setmetatable(global.l, -2);
	lua_setglobal(global.l, name2);

	print("add fun %s with id %d with sig %s", name2, functionId, signature2);

	(*env)->ReleaseStringUTFChars(env, name, name2);
	(*env)->ReleaseStringUTFChars(env, signature, signature2);
}

JNIEXPORT void JNICALL addFunc(JNIEnv* env, jobject this, jclass class, jstring name, jstring signature, int rtnType,
							   int argc)
{
	const char* name2 = (*env)->GetStringUTFChars(env, name, NULL);
	const char* signature2 = (*env)->GetStringUTFChars(env, signature, NULL);

	global.env = env;

	unsigned long long functionId = dynList_size(global.functions);
	dynList_resize((void**)&global.functions, functionId + 1);
	Function* fun = global.functions + functionId;

	function_initX(fun, class, name2, signature2, rtnType, argc);

	jstring className2 = getClassName(class);
	const char* className = (*env)->GetStringUTFChars(env, className2, NULL);

	lua_getglobal(global.l, className);

	if (lua_type(global.l, -1) == LUA_TNIL)
	{
		lua_pop(global.l, -1);
		lua_newtable(global.l);
	}

	lua_newtable(global.l);
	lua_pushinteger(global.l, functionId);
	lua_setfield(global.l, -2, "id");
	luaL_getmetatable(global.l, "luaFunc2");
	lua_setmetatable(global.l, -2);
	lua_setfield(global.l, -2, name2);
	lua_setglobal(global.l, className);

	print("add class fun %s with id %d with sig %s", name2, functionId, signature2);

	(*env)->ReleaseStringUTFChars(env, name, name2);
	(*env)->ReleaseStringUTFChars(env, signature, signature2);
	(*env)->ReleaseStringUTFChars(env, className2, className);
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
			if (type != fun->argTypes[i])
			{
				if (type != LUA_TTABLE || fun->argTypes[i] != LUA_TSTRING)
				{
					const char* msg;
					const char* typearg;
					if (luaL_getmetafield(l, i + 2 + s, "__name") == LUA_TSTRING)
						typearg = lua_tostring(l, -1);
					else if (lua_type(l, i + 2 + s) == LUA_TLIGHTUSERDATA)
						typearg = "light userdata";
					else
						typearg = luaL_typename(l, i + 2 + s);
					msg = lua_pushfstring(l, "%s expected, got %s", lua_typename(l, fun->argTypes[i]), typearg);
					luaL_argerror(l, i + 1, msg);
					return 0;
				}
			}
			switch (type)
			{
			case LUA_TNUMBER:
				args[i].d = lua_tonumber(l, i + 2 + s);
				break;
			case LUA_TBOOLEAN:
				args[i].z = lua_toboolean(l, i + 2 + s);
				break;
			case LUA_TSTRING:
				const char* str = lua_tostring(l, i + 2 + s);
				args[i].l = (*global.env)->NewStringUTF(global.env, str);
				break;
			case LUA_TTABLE:
				lua_getfield(l, i + 2 + s, "ref");
				args[i].l = lua_touserdata(l, -1);
				lua_pop(l, 1);
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
		function_callVX(fun, obj, args);
		free(args);
		return 0;
	}
	case -1: {
		function_callVX(fun, obj, args);
		free(args);
		lua_pushvalue(l, 2);
		return 1;
	}
	case LUA_TNUMBER: {
		double rtn = function_callDX(fun, obj, args);
		lua_pushnumber(l, rtn);
		free(args);
		return 1;
	}
	case LUA_TBOOLEAN: {
		char rtn = function_callBX(fun, obj, args);
		lua_pushboolean(l, rtn);
		free(args);
		return 1;
	}
	case LUA_TSTRING: {
		jstring rtn = function_callX(fun, obj, args);
		const char* str = (*global.env)->GetStringUTFChars(global.env, rtn, NULL);
		lua_pushstring(l, str);
		(*global.env)->ReleaseStringUTFChars(global.env, rtn, str);
		free(args);
		return 1;
	}
	case LUA_TTABLE: {
		jobject res = function_callX(fun, obj, args);
		jstring str = getClassName((*global.env)->GetObjectClass(global.env, res));
		const char* s = (*global.env)->GetStringUTFChars(global.env, str, NULL);
		free(args);

		lua_getglobal(l, s);
		int objPos = lua_gettop(l);
		if (lua_type(l, -1) != LUA_TTABLE)
			luaL_error(l, "attempted to return object of an unknown type \'%s\'", s);

		jobject ref = (*global.env)->NewGlobalRef(global.env, res);

		int objectId = dynList_size(global.objects);
		dynList_resize((void**)&global.objects, objectId + 1);
		global.objects[objectId] = ref;

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
		return 1;
	}
	}
	return 0;
}

int callFunc(lua_State* l)
{
	lua_getfield(l, 1, "id");
	int id = lua_tointeger(l, -1);
	lua_pop(l, 1);
	Function* fun = global.functions + id;

	jvalue* args = checkArgs(l, fun, 0);
	return call(l, fun, fun->obj, args);
}

int callFunc2(lua_State* l)
{
	lua_getfield(l, 1, "id");
	int id = lua_tointeger(l, -1);
	lua_pop(l, 1);

	Function* fun = global.functions + id;

	jvalue* args = checkArgs(l, fun, 1);

	lua_getfield(l, 2, "ref");
	if (lua_type(l, -1) != LUA_TLIGHTUSERDATA)
		luaL_error(l, "attempted to call function on a nil object");
	jobject ref = lua_touserdata(l, -1);
	lua_pop(l, 1);
	if ((*global.env)->IsSameObject(global.env, ref, NULL))
		luaL_error(l, "internal handle to jobject is null");

	int res = call(l, fun, ref, args);
	if ((*global.env)->ExceptionCheck(global.env))
		(*global.env)->ExceptionDescribe(global.env);
	return res;
}
