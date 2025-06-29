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
	jstring j = (*global.env)->NewStringUTF(global.env, buf);
	(*global.env)->CallVoidMethod(global.env, obj, printId, j);
}

void err(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vsnprintf(buf, 256, fmt, va);
	jstring j = (*global.env)->NewStringUTF(global.env, buf);
	(*global.env)->CallVoidMethod(global.env, obj, errId, j);
}

void initUtils(jobject object)
{
	if (obj)
		(*global.env)->DeleteGlobalRef(global.env, obj);
	else
	{
		jclass class = (*global.env)->GetObjectClass(global.env, object);
		printId = (*global.env)->GetMethodID(global.env, class, "print", "(Ljava/lang/String;)V");
		errId = (*global.env)->GetMethodID(global.env, class, "err", "(Ljava/lang/String;)V");
		jclass c = (*global.env)->GetObjectClass(global.env, class);
		getClassId = (*global.env)->GetMethodID(global.env, c, "getSimpleName", "()Ljava/lang/String;");
		buf = malloc(256);
	}
	obj = (*global.env)->NewGlobalRef(global.env, object);
}


jstring getClassName(jclass class)
{
	return (*global.env)->CallObjectMethod(global.env, class, getClassId);
}
