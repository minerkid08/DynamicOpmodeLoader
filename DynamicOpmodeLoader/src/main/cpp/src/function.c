#include "function.h"
#include "error.h"
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
		const char* str2 = getClassName(class);
		fbErr("can't find function with signature '%s.%s%s'", str2, name, sig);
		free((void*)str2);
	}

	this->obj = 0;
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
			while (sig[++i] != ';')
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

void function_initStaticX(Function* this, jclass class, const char* name, const char* sig, char rtnType, char argc)
{
	this->funId = (*env)->GetStaticMethodID(env, class, name, sig);

	if ((*env)->ExceptionCheck(env))
	{
		(*env)->ExceptionDescribe(env);
		const char* str2 = getClassName(class);
		fbErr("can't find function with signature '%s.%s%s'", str2, name, sig);
		free((void*)str2);
	}

	this->obj = class;
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

void function_init(Function *this, jobject obj, const char *name, const char *sig, char rtnType, char argc)
{
  jclass class = (*env)->GetObjectClass(env, obj);
	this->funId = (*env)->GetMethodID(env, class, name, sig);

	if ((*env)->ExceptionCheck(env))
	{
		(*env)->ExceptionDescribe(env);
		const char* str2 = getClassName(class);
		fbErr("can't find function with signature '%s.%s%s'", str2, name, sig);
		free((void*)str2);
	}

	this->obj = obj;
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
