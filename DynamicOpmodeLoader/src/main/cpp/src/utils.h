#pragma once
#include "jni.h"

void print(const char* fmt, ...);
void luaErr(const char* msg);
void cpErr(const char* msg);
void fbErr(const char* fmt, ...);
void opErr(const char* fmt, ...);
void initUtils(jobject object);

jstring getClassName(jclass class);
char strStartsWith(const char* a, const char* b);
