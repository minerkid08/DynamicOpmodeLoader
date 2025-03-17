#include "utils.h"
#include "global.h"
#include "jni.h"
#include <malloc.h>

jobject obj;
jmethodID printId;
jmethodID errId;
char* buf;

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
		buf = malloc(256);
	}
	obj = (*global.env)->NewGlobalRef(global.env, object);
}

static jmethodID getClassFun = 0;

jstring getClassName(jclass class)
{
	if (!getClassFun)
	{
		jclass c = (*global.env)->GetObjectClass(global.env, class);
		getClassFun = (*global.env)->GetMethodID(global.env, c, "getSimpleName", "()Ljava/lang/String;");
	}
	return (*global.env)->CallObjectMethod(global.env, class, getClassFun);
}
