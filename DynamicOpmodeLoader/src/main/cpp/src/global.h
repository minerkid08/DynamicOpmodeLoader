#pragma once

#include "jni.h"
#include "lua/lua.h"

typedef struct
{
	int id;
	char* name;
} Opmode;

extern int currentOpmode;
extern lua_State* l;
extern JNIEnv* env;
extern Opmode* opmodes;
