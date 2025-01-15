#include "function.h"
#include "global.h"
#include "jni.h"
#include "lua/lua.h"
#include <stdlib.h>
#include <string.h>

void function_init(Function* this, const char* name, const char* sig, char rtnType, char argc)
{
	jclass class = (*global.env)->GetObjectClass(global.env, global.currentObject);
	this->funId = (*global.env)->GetMethodID(global.env, class, name, sig);
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
			this->argTypes[currentArg++] = LUA_TSTRING;
		if (c == 'D')
			this->argTypes[currentArg++] = LUA_TNUMBER;
		if (currentArg == argc)
			break;
	}
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
