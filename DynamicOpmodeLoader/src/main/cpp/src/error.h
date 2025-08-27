#pragma once

void initError();
void luaErr(const char* msg);
void cpErr(const char* msg);
void fbErr(const char* fmt, ...);
void opErr(const char* fmt, ...);
