#include "utils.h"
#include "global.h"
#include "jni.h"
#include <malloc.h>

static jobject obj = 0;
static jmethodID printId;
static jmethodID luaErrId;
static jmethodID cpErrId;
static jmethodID fbErrId;
static jmethodID opErrId;
static jmethodID getClassId;
static char* buf;

void print(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vsnprintf(buf, 256, fmt, va);
	jstring j = (*env)->NewStringUTF(env, buf);
	(*env)->CallVoidMethod(env, obj, printId, j);
}

void luaErr(const char* msg)
{
	jstring j = (*env)->NewStringUTF(env, msg);
	(*env)->CallVoidMethod(env, obj, luaErrId, j);
}

void cpErr(const char* msg)
{
	jstring j = (*env)->NewStringUTF(env, msg);
	(*env)->CallVoidMethod(env, obj, cpErrId, j);
}

void fbErr(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vsnprintf(buf, 256, fmt, va);
	jstring j = (*env)->NewStringUTF(env, buf);
	(*env)->CallVoidMethod(env, obj, fbErrId, j);
}

void opErr(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vsnprintf(buf, 256, fmt, va);
	jstring j = (*env)->NewStringUTF(env, buf);
	(*env)->CallVoidMethod(env, obj, opErrId, j);
}

void initUtils(jobject object)
{
	if (obj == 0)
	{
		jclass class = (*env)->GetObjectClass(env, object);
		printId = (*env)->GetMethodID(env, class, "print", "(Ljava/lang/String;)V");
		luaErrId = (*env)->GetMethodID(env, class, "luaErr", "(Ljava/lang/String;)V");
		cpErrId = (*env)->GetMethodID(env, class, "cpErr", "(Ljava/lang/String;)V");
		fbErrId = (*env)->GetMethodID(env, class, "fbErr", "(Ljava/lang/String;)V");
		opErrId = (*env)->GetMethodID(env, class, "opmodeErr", "(Ljava/lang/String;)V");
		jclass c = (*env)->GetObjectClass(env, class);
		getClassId = (*env)->GetMethodID(env, c, "getSimpleName", "()Ljava/lang/String;");
		buf = malloc(256);
		obj = (*env)->NewGlobalRef(env, object);
	}
}

jstring getClassName(jclass class)
{
	return (*env)->CallObjectMethod(env, class, getClassId);
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
