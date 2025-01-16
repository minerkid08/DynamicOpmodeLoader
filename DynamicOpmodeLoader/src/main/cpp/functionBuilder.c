#include "functionBuilder.h"
#include "dynList.h"
#include "function.h"
#include "global.h"
#include "jni.h"
#include "jni_md.h"
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define addObject Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_setCurrentObject
#define addFun Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_addFunction

int callFunc(lua_State* l);

void initFunctionBuilder()
{
	global.objects = dynList_new(5, sizeof(jobject));
	dynList_resize((void**)&global.objects, 0);

	global.functions = dynList_new(10, sizeof(Function));
	dynList_resize((void**)&global.functions, 0);

	luaL_newmetatable(global.l, "luaFunc");
	lua_pushcfunction(global.l, callFunc);
	lua_setfield(global.l, -2, "__call");
}

void reset()
{
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

	function_init(fun, name2, signature2, rtnType, argc);

	lua_newtable(global.l);
	lua_pushinteger(global.l, functionId);
	lua_setfield(global.l, -2, "id");
	luaL_getmetatable(global.l, "luaFunc");
	lua_setmetatable(global.l, -2);
	lua_setglobal(global.l, name2);

	print("add fun %s with id %d with sig %s\n", name2, functionId, signature2);

	(*env)->ReleaseStringUTFChars(env, name, name2);
	(*env)->ReleaseStringUTFChars(env, signature, signature2);
}

int callFunc(lua_State* l)
{
	int argc = lua_gettop(l) - 1;
	lua_getfield(l, 1, "id");
	int id = lua_tointeger(l, -1);
	Function* fun = global.functions + id;
	if (fun->argc != argc)
	{
		luaL_error(l, "expected %d args, got %d\n", fun->argc, argc);
	}

	jvalue* args = 0;
	if (argc > 0)
	{
		args = malloc(sizeof(jvalue) * argc);

		for (int i = 0; i < argc; i++)
		{
			char type = lua_type(l, i + 2);
			if (type != fun->argTypes[i])
			{
				const char* msg;
				const char* typearg;
				if (luaL_getmetafield(l, i + 2, "__name") == LUA_TSTRING)
					typearg = lua_tostring(l, -1);
				else if (lua_type(l, i + 2) == LUA_TLIGHTUSERDATA)
					typearg = "light userdata";
				else
					typearg = luaL_typename(l, i + 2);
				msg = lua_pushfstring(l, "%s expected, got %s", luaL_typename(l, fun->argTypes[i]), typearg);
				luaL_argerror(l, i + 1, msg);
				return 0;
			}
			switch (type)
			{
			case LUA_TNUMBER:
				args[i].d = lua_tonumber(l, i + 2);
				break;
			case LUA_TBOOLEAN:
				args[i].z = lua_toboolean(l, i + 2);
				break;
			case LUA_TSTRING:
				const char* str = lua_tostring(l, i + 2);
				args[i].l = (*global.env)->NewStringUTF(global.env, str);
				break;
			}
		}
	}
	switch (fun->rtnType)
	{
	case LUA_TNONE: {
		char rtn = function_callB(fun, args);
		free(args);
		if (rtn)
		{
			luaL_error(l, "opmode stopped :)");
		}
		return 0;
	}
	case LUA_TNIL: {
		function_callV(fun, args);
		free(args);
		return 0;
	}
	case LUA_TNUMBER: {
		double rtn = function_callD(fun, args);
		lua_pushnumber(l, rtn);
		free(args);
		return 1;
	}
	case LUA_TBOOLEAN: {
		char rtn = function_callB(fun, args);
		lua_pushboolean(l, rtn);
		free(args);
		return 1;
	}
	case LUA_TSTRING:
		jstring rtn = function_call(fun, args);
		const char* str = (*global.env)->GetStringUTFChars(global.env, rtn, NULL);
		lua_pushstring(l, str);
		(*global.env)->ReleaseStringUTFChars(global.env, rtn, str);
		free(args);
		return 1;
	}
	return 0;
}
