#include "error.h"
#include "global.h"
#include "stdio.h"

#include <jni.h>

#define LuaErrorClassName "com/minerkid08/dynamicopmodeloader/LuaError"
#define CompileErrorClassName "com/minerkid08/dynamicopmodeloader/CompileError"
#define FunctionBuilderErrorClassName "com/minerkid08/dynamicopmodeloader/FunctionBuilderError"
#define OpmodeErrorClassName "com/minerkid08/dynamicopmodeloader/UndefinedOpmodeError"

typedef struct
{
	jclass class;
	jmethodID constructor;
} ErrorDef;

ErrorDef luaError;
ErrorDef compileError;
ErrorDef functionBuilderError;
ErrorDef opmodeError;

void genError(ErrorDef* error, const char* className)
{
	error->class = (*env)->FindClass(env, className);
	error->constructor = (*env)->GetMethodID(env, error->class, "<init>", "(Ljava/lang/String;)V");
  error->class = (*env)->NewGlobalRef(env, error->class);
}

void initError()
{
	genError(&luaError, LuaErrorClassName);
	genError(&compileError, CompileErrorClassName);
	genError(&functionBuilderError, FunctionBuilderErrorClassName);
	genError(&opmodeError, OpmodeErrorClassName);
}

void luaErr(const char* msg)
{
	jstring str = (*env)->NewStringUTF(env, msg);

	jobject j = (*env)->NewObject(env, luaError.class, luaError.constructor, str);

	(*env)->Throw(env, j);
}

void cpErr(const char* msg)
{
	jstring str = (*env)->NewStringUTF(env, msg);

	jobject j = (*env)->NewObject(env, compileError.class, compileError.constructor, str);

	(*env)->Throw(env, j);
}

static char buf[256];

void fbErr(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vsnprintf(buf, 256, fmt, va);

	jstring str = (*env)->NewStringUTF(env, buf);

	jobject j = (*env)->NewObject(env, functionBuilderError.class, functionBuilderError.constructor, str);

	(*env)->Throw(env, j);
}

void opErr(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vsnprintf(buf, 256, fmt, va);

	jstring str = (*env)->NewStringUTF(env, buf);

	jobject j = (*env)->NewObject(env, opmodeError.class, opmodeError.constructor, str);

	(*env)->Throw(env, j);
}
