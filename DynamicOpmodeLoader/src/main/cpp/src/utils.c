#include "utils.h"
#include "global.h"
#include "jni.h"
#include <malloc.h>
#include <string.h>

static jobject obj = 0;
static jmethodID printId;
static jmethodID getClassId;
static char buf[256];

void initUtils()
{
	if (obj == 0)
	{
		jclass class = (*env)->FindClass(env, "com/minerkid08/dynamicopmodeloader/LuaStdlib");
		if ((*env)->ExceptionCheck(env))
		{
			(*env)->ExceptionDescribe(env);
			(*env)->ExceptionClear(env);
		}
		printId = (*env)->GetStaticMethodID(env, class, "print", "(Ljava/lang/String;)V");
		if ((*env)->ExceptionCheck(env))
		{
			(*env)->ExceptionDescribe(env);
			(*env)->ExceptionClear(env);
		}
		jclass c = (*env)->GetObjectClass(env, class);
		if ((*env)->ExceptionCheck(env))
		{
			(*env)->ExceptionDescribe(env);
			(*env)->ExceptionClear(env);
		}
		getClassId = (*env)->GetMethodID(env, c, "getSimpleName", "()Ljava/lang/String;");
		if ((*env)->ExceptionCheck(env))
		{
			(*env)->ExceptionDescribe(env);
			(*env)->ExceptionClear(env);
		}
		obj = (*env)->NewGlobalRef(env, class);
	}
}

void print(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vsnprintf(buf, 256, fmt, va);

	jstring str = (*env)->NewStringUTF(env, buf);

	(*env)->CallStaticVoidMethod(env, obj, printId, str);
}

const char* getClassName(jclass class)
{
	jstring name2 = (*env)->CallObjectMethod(env, class, getClassId);
	const char* name = (*env)->GetStringUTFChars(env, name2, 0);
	
	int len = strlen(name);
	int start = 0;
	int end = len;
	for(int i = 0; i < len; i++)
	{
		if(name[i] == '.')
			start = i;
		if(name[i] == '$')
			end = i;
	}

	len = end - start;
	char* n = malloc(len + 1);
	memcpy(n, name, len);
	n[len] = 0;
	(*env)->ReleaseStringUTFChars(env, name2, name);
	return n;
}

char strStartsWith(const char* a, const char* b)
{
	int i = 0;
	char ca = b[i];
	while (ca != 0)
	{
		if (ca != a[i])
			return 0;
		i++;
		ca = b[i];
	}
	return 1;
}
