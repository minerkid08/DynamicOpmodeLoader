#include "utils.h"
#include "global.h"
#include "jni.h"
#include <malloc.h>

static jobject obj;
static jmethodID printId;
static jmethodID errId;
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

void err(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vsnprintf(buf, 256, fmt, va);
	jstring j = (*env)->NewStringUTF(env, buf);
	(*env)->CallVoidMethod(env, obj, errId, j);
}

void initUtils(jobject object)
{
	if (obj)
		(*env)->DeleteGlobalRef(env, obj);
	else
	{
		jclass class = (*env)->GetObjectClass(env, object);
		printId = (*env)->GetMethodID(env, class, "print", "(Ljava/lang/String;)V");
		errId = (*env)->GetMethodID(env, class, "err", "(Ljava/lang/String;)V");
		jclass c = (*env)->GetObjectClass(env, class);
		getClassId = (*env)->GetMethodID(env, c, "getSimpleName", "()Ljava/lang/String;");
		buf = malloc(256);
	}
	obj = (*env)->NewGlobalRef(env, object);
}


jstring getClassName(jclass class)
{
	return (*env)->CallObjectMethod(env, class, getClassId);
}
