#include "utils.h"

jobject obj;
jmethodID printId;
jmethodID errId;

void print(const char* fmt, ...);
void err(const char* fmt, ...);
void initUtils(jobject object);
