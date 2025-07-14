#pragma once

#include "jni.h"
#include "lua/lua.h"

#include "function.h"

typedef struct
{
	int id;
	char* name;
} Opmode;

extern int currentOpmode;
extern lua_State* l;
extern JNIEnv* env;
extern Opmode* opmodes;
extern Function* functions;
extern jobject* objects;
extern jobject currentObject;
