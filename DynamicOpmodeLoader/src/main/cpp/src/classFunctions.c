#include "dynList.h"
#include "function.h"
#include "functionBuilder.h"
#include "global.h"
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "utils.h"
#include <jni.h>

#define createClass Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_createClass
#define addFunc Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_addFunctionc

static Function* classFunctions;

int objectGC(lua_State* l);
static int callClassFunc(lua_State* l);

void initClassFunctions()
{
	classFunctions = dynList_new(0, sizeof(Function));
	dynList_reserve((void**)&classFunctions, 25);
}

void initClassFunctionsLua()
{
	luaL_newmetatable(l, "jclassfunc");
	lua_pushcfunction(l, callClassFunc);
	lua_setfield(l, -2, "__call");

	luaL_newmetatable(l, "jobject");
	lua_pushcfunction(l, objectGC);
	lua_setfield(l, -2, "__gc");
}

void resetClassFunctions()
{
	dynList_resize((void**)&classFunctions, 0);
}

JNIEXPORT void JNICALL createClass(JNIEnv* env, jobject this, jstring str)
{
	const char* s = (*env)->GetStringUTFChars(env, str, NULL);
	lua_newtable(l);
	lua_setglobal(l, s);
	(*env)->ReleaseStringUTFChars(env, str, s);
}

JNIEXPORT void JNICALL addFunc(JNIEnv* env2, jobject this, jclass class, jstring name, jstring signature, int rtnType,
							   int argc)
{
	env = env2;

	const char* name2 = (*env)->GetStringUTFChars(env, name, NULL);
	const char* signature2 = (*env)->GetStringUTFChars(env, signature, NULL);

	unsigned long long functionId = dynList_size(classFunctions);
	dynList_resize((void**)&classFunctions, functionId + 1);
	Function* fun = classFunctions + functionId;

	char rtnType2 = rtnType;
	if (rtnType == -1)
		rtnType2 = -1;
	function_initX(fun, class, name2, signature2, rtnType2, argc);

	jstring className2 = getClassName(class);
	const char* className = (*env)->GetStringUTFChars(env, className2, NULL);

	lua_getglobal(l, className);

	if (lua_type(l, -1) == LUA_TNIL)
	{
		lua_pop(l, 1);
		lua_newtable(l);
	}

	lua_newtable(l);
	lua_pushinteger(l, functionId);
	lua_setfield(l, -2, "id");
	luaL_getmetatable(l, "jclassfunc");
	lua_setmetatable(l, -2);
	lua_setfield(l, -2, name2);
	lua_setglobal(l, className);

	print("add class fun %s with id %d with sig %s", name2, functionId, signature2);

	(*env)->ReleaseStringUTFChars(env, name, name2);
	(*env)->ReleaseStringUTFChars(env, signature, signature2);
	(*env)->ReleaseStringUTFChars(env, className2, className);
}

int callClassFunc(lua_State* l)
{
	lua_getfield(l, 1, "id");
	int id = lua_tointeger(l, -1);
	lua_pop(l, 1);

	Function* fun = classFunctions + id;

	jvalue* args = checkArgs(l, fun, 1);

	lua_getfield(l, 2, "ref");
	if (lua_type(l, -1) != LUA_TLIGHTUSERDATA)
		luaL_error(l, "attempted to call function on a nil object");
	jobject ref = lua_touserdata(l, -1);
	lua_pop(l, 1);
	if ((*env)->IsSameObject(env, ref, NULL))
		luaL_error(l, "internal handle to jobject is null");

	int res = call(l, fun, ref, args);
	if ((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	return res;
}

int objectGC(lua_State* l)
{
	lua_getfield(l, -1, "ref");
	if (lua_type(l, -1) != LUA_TLIGHTUSERDATA)
		luaL_error(l, "attempted to free invalid object");
	jobject ref = lua_touserdata(l, -1);
	(*env)->DeleteGlobalRef(env, ref);
	return 0;
}
