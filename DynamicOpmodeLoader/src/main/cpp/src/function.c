#include "function.h"
#include "global.h"
#include "jni.h"
#include "lua/lua.h"
#include <stdlib.h>
#include <string.h>

void function_initX(Function* this, jclass class, const char* name, const char* sig, char rtnType, char argc)
{
	this->funId = (*global.env)->GetMethodID(global.env, class, name, sig);

	if ((*global.env)->ExceptionCheck(global.env))
	{
		(*global.env)->ExceptionDescribe(global.env);
		exit(1);
	}

	this->obj = global.currentObject;
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
			this->argTypes[currentArg++] = LUA_TSTRING;
			while (sig[++i] == ';')
				;
		}
		if (c == 'D')
			this->argTypes[currentArg++] = LUA_TNUMBER;
		if (currentArg == argc)
			break;
	}
}

void function_init(Function* this, const char* name, const char* sig, char rtnType, char argc)
{
	jclass class = (*global.env)->GetObjectClass(global.env, global.currentObject);
	function_initX(this, class, name, sig, rtnType, argc);
}

void function_callV(Function* this, jvalue* args)
{
	(*global.env)->CallVoidMethodA(global.env, this->obj, this->funId, args);
}
double function_callD(Function* this, jvalue* args)
{
	return (*global.env)->CallDoubleMethodA(global.env, this->obj, this->funId, args);
}
char function_callB(Function* this, jvalue* args)
{
	return (*global.env)->CallBooleanMethodA(global.env, this->obj, this->funId, args);
}
jobject function_call(Function* this, jvalue* args)
{
	return (*global.env)->CallObjectMethodA(global.env, this->obj, this->funId, args);
}

void function_callVX(Function* this, jobject object, jvalue* args)
{
	(*global.env)->CallVoidMethodA(global.env, object, this->funId, args);
}

double function_callDX(Function* this, jobject object, jvalue* args)
{
	return (*global.env)->CallDoubleMethodA(global.env, object, this->funId, args);
}

char function_callBX(Function* this, jobject object, jvalue* args)
{
	return (*global.env)->CallBooleanMethodA(global.env, object, this->funId, args);
}

jobject function_callX(Function* this, jobject object, jvalue* args)
{
	return (*global.env)->CallObjectMethodA(global.env, object, this->funId, args);
}
