#include "defFile.h"
#include "dynList.h"
#include "function.h"
#include "functionBuilder.h"
#include "global.h"
#include "utils.h"
#include <jni.h>
#include <lua/lauxlib.h>
#include <lua/lua.h>

#define addObject Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_setCurrentObject
#define addFun Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_addFunction

static Function* functions;
static jobject* objects;
static jobject currentObject;

static int callFunc(lua_State* l);

void initGlobalFunctions()
{
	functions = dynList_new(0, sizeof(Function));
	dynList_reserve((void**)&functions, 25);
	objects = dynList_new(0, sizeof(jobject));
	dynList_reserve((void**)&objects, 5);
}

void initGlobalFunctionsLua()
{
	luaL_newmetatable(l, "jfunc");
	lua_pushcfunction(l, callFunc);
	lua_setfield(l, -2, "__call");
}

void resetGlobalFunctions()
{
	int s = dynList_size(objects);
	for (int i = 0; i < s; i++)
	{
		if (objects[i] != 0)
			(*env)->DeleteGlobalRef(env, objects[i]);
	}
	dynList_resize((void**)&objects, 0);
	dynList_resize((void**)&functions, 0);
}

JNIEXPORT void JNICALL addObject(JNIEnv* env2, jobject this, jobject object)
{
	env = env2;

	jobject obj = (*env)->NewGlobalRef(env, object);
	currentObject = obj;

	int objectId = dynList_size(objects);
	dynList_resize((void**)&objects, objectId + 1);
	*(objects + objectId) = obj;
}

JNIEXPORT void JNICALL addFun(JNIEnv* env2, jobject this, jstring name, jstring signature, int rtnType, int argc)
{
	env = env2;

	const char* name2 = (*env)->GetStringUTFChars(env, name, NULL);
	const char* signature2 = (*env)->GetStringUTFChars(env, signature, NULL);

	unsigned long long functionId = dynList_size(functions);
	dynList_resize((void**)&functions, functionId + 1);
	Function* fun = functions + functionId;

	function_init(fun, currentObject, name2, signature2, rtnType, argc);

	lua_newtable(l);
	lua_pushinteger(l, functionId);
	lua_setfield(l, -2, "id");
	luaL_getmetatable(l, "jfunc");
	lua_setmetatable(l, -2);
	if (tableLevel > 0)
		lua_setfield(l, -2, name2);
	else
		lua_setglobal(l, name2);

	addf(name2, signature2);

	print("add fun %s with id %d with sig %s", name2, functionId, signature2);

	(*env)->ReleaseStringUTFChars(env, name, name2);
	(*env)->ReleaseStringUTFChars(env, signature, signature2);
}

int callFunc(lua_State* l)
{
	lua_getfield(l, 1, "id");
	int id = lua_tointeger(l, -1);
	lua_pop(l, 1);
	Function* fun = functions + id;

	jvalue* args = checkArgs(l, fun, 0);
	return call(l, fun, fun->obj, args);
}
