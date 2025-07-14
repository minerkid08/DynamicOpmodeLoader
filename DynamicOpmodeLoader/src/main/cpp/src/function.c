#include "function.h"
#include "global.h"
#include "jni.h"
#include "lua/lua.h"
#include "type.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

void function_initX(Function* this, jclass class, const char* name, const char* sig, char rtnType, char argc)
{
	this->funId = (*env)->GetMethodID(env, class, name, sig);

	if ((*env)->ExceptionCheck(env))
	{
		(*env)->ExceptionDescribe(env);
		err("can't find function with signature '%s'", sig);
	}

	this->obj = currentObject;
	this->argc = argc;
	this->rtnType = rtnType;
	this->argTypes = malloc(argc);
	char currentArg = 0;
	for (int i = 0; i < strlen(sig); i++)
	{
		char c = sig[i];
		if (c == ')')
			break;
		if (c == 'Z')
			this->argTypes[currentArg++] = LUA_TBOOLEAN;
		if (c == 'L')
		{
			if (strStartsWith(sig + i + 1, "java/lang/String"))
				this->argTypes[currentArg++] = LUA_TSTRING;
			else if (strStartsWith(sig + i + 1, "com/minerkid08/dynamicopmodeloader/LuaCallback"))
				this->argTypes[currentArg++] = LUA_TFUNCTION;
			else
				this->argTypes[currentArg++] = LUA_TTABLE;
			while (sig[++i] == ';')
				;
		}
		if (c == 'D')
			this->argTypes[currentArg++] = LUA_TNUMBER;
		if (c == 'I')
			this->argTypes[currentArg++] = TINT;
		if (c == 'F')
			this->argTypes[currentArg++] = TFLOAT;
		if (currentArg == argc)
			break;
	}
}

void function_init(Function* this, const char* name, const char* sig, char rtnType, char argc)
{
	jclass class = (*env)->GetObjectClass(env, currentObject);
	function_initX(this, class, name, sig, rtnType, argc);
}

void function_callV(Function* this, jvalue* args)
{
	(*env)->CallVoidMethodA(env, this->obj, this->funId, args);
}
double function_callD(Function* this, jvalue* args)
{
	return (*env)->CallDoubleMethodA(env, this->obj, this->funId, args);
}
char function_callB(Function* this, jvalue* args)
{
	return (*env)->CallBooleanMethodA(env, this->obj, this->funId, args);
}
jobject function_call(Function* this, jvalue* args)
{
	return (*env)->CallObjectMethodA(env, this->obj, this->funId, args);
}

void function_callVX(Function* this, jobject object, jvalue* args)
{
	(*env)->CallVoidMethodA(env, object, this->funId, args);
}

double function_callDX(Function* this, jobject object, jvalue* args)
{
	return (*env)->CallDoubleMethodA(env, object, this->funId, args);
}

float function_callFX(Function* this, jobject object, jvalue* args)
{
	return (*env)->CallFloatMethodA(env, object, this->funId, args);
}

int function_callIX(Function* this, jobject object, jvalue* args)
{
	return (*env)->CallIntMethodA(env, object, this->funId, args);
}

char function_callBX(Function* this, jobject object, jvalue* args)
{
	return (*env)->CallBooleanMethodA(env, object, this->funId, args);
}

jobject function_callX(Function* this, jobject object, jvalue* args)
{
	return (*env)->CallObjectMethodA(env, object, this->funId, args);
}
