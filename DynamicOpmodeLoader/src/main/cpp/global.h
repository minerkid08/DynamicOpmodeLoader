#pragma once

#include "jni.h"
#include "lua/lua.h"

#include "function.h"

typedef struct
{
  int id;
	char* name;
} Opmode;

typedef struct
{
  int currentOpmode;
  lua_State* l;
  JNIEnv* env;
  Opmode* opmodes;
  Function* functions;
  jobject* objects;
  jobject currentObject;
} Global;

extern Global global;
