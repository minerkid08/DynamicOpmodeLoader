#pragma once

#include "jni.h"
#include "lua/lua.h"

typedef struct
{
	int id;
	char* name;
  int type;
  char* group;
  char* order;
} Opmode;

extern int currentOpmode;
extern lua_State* l;
extern JNIEnv* env;
extern Opmode* opmodes;

#define MODE_NORMAL 0
#define MODE_GENFILE 1

extern int mode;
