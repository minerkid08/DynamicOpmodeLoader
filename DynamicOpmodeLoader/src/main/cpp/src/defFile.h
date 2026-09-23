#pragma once

void initDefFile();
void closeDefFile();

void addClass(const char* name);
void addClassFun(const char* className, const char* funName, const char* funSig, char builder);

void addTable(const char* name);
void removeTable();
void addi(const char* name, int value);
void addd(const char* name, double value);
void addb(const char* name, char value);
void adds(const char* name, const char* value);
void addo(const char* name, const char* objName);
void addf(const char* name, const char* signature);
