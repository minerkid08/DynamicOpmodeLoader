#pragma once

#ifdef _WIN64
#define print printf
#define err(fmt, ...) printf(fmt, __VA_ARGS__); fflush(stdout); exit(1)
#else
void print(const char* str, ...);
void err(const char* str, ...);
#endif
void initUtils();
