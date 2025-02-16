#pragma once
#include "jni.h"

void print(const char* fmt, ...);
void err(const char* fmt, ...);
void initUtils(jobject object);

jstring getClassName(jclass class);
