#pragma once

#include "jni.h"
typedef struct
{
	char argc;
	char rtnType;
	char* argTypes;

	jmethodID funId;
	jobject obj;
} Function;

void function_initX(Function* this, jclass class, const char* name, const char* sig, char rtnType, char argc);
void function_initStaticX(Function* this, jclass class, const char* name, const char* sig, char rtnType, char argc);

void function_init(Function* this, jobject obj, const char* name, const char* sig, char rtnType, char argc);
