#include "vararg.h"
#include "global.h"
#include "utils.h"
#include "string.h"

static jmethodID getBool = 0L;
static jmethodID getInt = 0L;
static jmethodID getDouble = 0L;

void pushArgs(jobjectArray args, int len)
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
