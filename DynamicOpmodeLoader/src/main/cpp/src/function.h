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

void function_init(Function* this, const char* name, const char* sig, char rtnType, char argc);
void function_callV(Function* this, jvalue* args);
double function_callD(Function* this, jvalue* args);
char function_callB(Function* this, jvalue* args);
jobject function_call(Function* this, jvalue* args);
