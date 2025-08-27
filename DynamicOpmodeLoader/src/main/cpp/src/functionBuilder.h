#pragma once

#include "function.h"
#include "jni.h"
#include "lua/lua.h"

extern int tableLevel;

void fbInit();
void fbInitLua();
void fbReset();

void initGlobalFunctions();
void initGlobalFunctionsLua();
void resetGlobalFunctions();

void initStaticFunctions();
void initStaticFunctionsLua();
void resetStaticFunctions();

void initClassFunctions();
void initClassFunctionsLua();
void resetClassFunctions();

jvalue* checkArgs(lua_State* l, Function* fun, int skip);
int call(lua_State* l, Function* fun, jobject ref, jvalue* args);
int callStatic(lua_State* l, Function* fun, jclass ref, jvalue* args);
