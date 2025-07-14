#include "callback.h"
#include "global.h"
#include "jni.h"
#include "jni_md.h"
#include "lua/lua.h"
#include "utils.h"
#include "vararg.h"

#define callCallback Java_com_minerkid08_dynamicopmodeloader_LuaCallback_call

static jclass class = 0;
static jmethodID constructor;
static jfieldID idField;

void initCallback()
{
	if (class == 0)
	{
		class = (*env)->FindClass(env, "com/minerkid08/dynamicopmodeloader/LuaCallback");
		constructor = (*env)->GetMethodID(env, class, "<init>", "(I)V");
		idField = (*env)->GetFieldID(env, class, "id", "I");
    class = (*env)->NewGlobalRef(env, class);
	}
}

static int callbackCount = 0;

void initCallbackLua()
{
	lua_newtable(l);
	lua_setglobal(l, "__callbacks");
	callbackCount = 0;
}

jobject makeCallback(int index)
{
	lua_getglobal(l, "__callbacks");
	lua_pushvalue(l, index);
	lua_seti(l, -2, callbackCount);
	lua_setglobal(l, "__callbacks");

	jobject object = (*env)->NewObject(env, class, constructor, callbackCount);
	callbackCount++;
	return object;
}

JNIEXPORT void JNICALL callCallback(JNIEnv* env2, jobject this, jobjectArray args)
{
	int id = (*env)->GetIntField(env, this, idField);

	lua_getglobal(l, "__callbacks");
	lua_geti(l, -1, id);
	if (lua_type(l, -1) == LUA_TFUNCTION)
	{
		int len = (*env)->GetArrayLength(env, args);
		pushArgs(args, len);
		if (lua_pcall(l, len, 0, 0))
			err("%s", lua_tostring(l, -1));
	}
	else
		err("invalid callback id used");
	lua_settop(l, 2);
}
