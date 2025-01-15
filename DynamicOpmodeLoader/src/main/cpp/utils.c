#include "utils.h"
#include "vadefs.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN64
void print(const char* str, ...)
{
	va_list args;
	printf(str, args);
}

void err(const char* str, ...)
{
	va_list args;
	printf(str, args);
	exit(1);
}

void initUtils()
{
}
#endif
