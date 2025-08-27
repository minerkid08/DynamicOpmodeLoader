#pragma once
#include "jni.h"

void print(const char* fmt, ...);
void initUtils();

jstring getClassName(jclass class);
char strStartsWith(const char* a, const char* b);
