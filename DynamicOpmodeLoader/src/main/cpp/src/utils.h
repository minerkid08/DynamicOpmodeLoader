#pragma once
#include "jni.h"

void print(const char* fmt, ...);
void initUtils();

const char* getClassName(jclass class);
char strStartsWith(const char* a, const char* b);
