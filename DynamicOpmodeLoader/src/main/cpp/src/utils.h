#pragma once

#ifdef _WIN64
#define print printf
#define err(fmt, ...) printf(fmt, __VA_ARGS__); fflush(stdout); exit(1)
#else
#define print(fmt, ...)
#define err(fmt, ...)
#endif
void initUtils();
