#include "lua/lauxlib.h"
#include "lua/lua.h"
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynList.h"

#define init Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_internalInit
#define loadOpmode Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_loadOpmode
#define start Java_com_minerkid08_dynamicopmodeloader_OpmodeLoader_start

typedef struct
{
	char* name;
} Opmode;

Opmode* opmodes;
int opmodeId = -1;
lua_State* l;

int print(lua_State* l)
{
	printf("%s\n", lua_tostring(l, -1));
	return 0;
}

int addOpmode(lua_State* l)
{
	int top = lua_gettop(l);
	if (top != 1)
		luaL_error(l, "expected 1 argument, got %d", top);
	if (lua_type(l, -1) != LUA_TTABLE)
		luaL_typeerror(l, 1, "table");

	int opmodeId = dynList_size(opmodes);
	dynList_resize((void**)&opmodes, opmodeId + 1);
	Opmode* opmode = opmodes + opmodeId;

	lua_getfield(l, -1, "name");
	const char* name = lua_tostring(l, -1);
	int stringLen = strlen(name);

	opmode->name = malloc(stringLen + 1);
	strcpy(opmode->name, name);
	opmode->name[stringLen] = 0;

	lua_pop(l, 1);

  lua_getglobal(l, "data");
  lua_pushvalue(l, -2);
  lua_seti(l, -2, opmodeId);
 // lua_setglobal(l, "data");

	return 0;
}

JNIEXPORT jobjectArray JNICALL init(JNIEnv* env, jobject this, jobject stdlib)
{
	opmodes = dynList_new(sizeof(Opmode), 5);
	dynList_resize((void*)&opmodes, 0);

	l = luaL_newstate();
	lua_pushcfunction(l, print);
	lua_setglobal(l, "print");

	lua_pushcfunction(l, addOpmode);
	lua_setglobal(l, "loadOpmode");

  lua_newtable(l);
  lua_setglobal(l, "data");

	if (luaL_dofile(l, "lua/init.lua"))
	{
		printf("something happened :(\n%s", lua_tostring(l, -1));
		return NULL;
	}
	int opmodeCount = dynList_size(opmodes);

	jobjectArray arr = (*env)->NewObjectArray(env, opmodeCount, (*env)->FindClass(env, "java/lang/String"), NULL);

	printf("loaded %d opmodes\n", opmodeCount);
	fflush(stdout);

	for (int i = 0; i < opmodeCount; i++)
	{
		Opmode* opmode = opmodes + i;
		jstring str = (*env)->NewStringUTF(env, opmode->name);
		(*env)->SetObjectArrayElement(env, arr, i, str);
		(*env)->DeleteLocalRef(env, str);
	}
	return arr;
}

JNIEXPORT void JNICALL loadOpmode(JNIEnv* env, jobject this, jstring opmodeName)
{
  opmodeId = -1;
	const char* name = (*env)->GetStringUTFChars(env, opmodeName, NULL);
	int opmodeCount = dynList_size(opmodes);
	for (int i = 0; i < opmodeCount; i++)
	{
		Opmode* opmode = opmodes + i;
		if (strcmp(opmode->name, name) == 0)
		{
			opmodeId = i;
			break;
		}
	}

	if (opmodeId == -1)
	{
		printf("cant find opmode %s", name);
		return;
	}


  lua_settop(l, 0);
  lua_getglobal(l, "data");
  lua_geti(l, -1, opmodeId);
  lua_getfield(l, -1, "init");
  if(lua_type(l, -1) == LUA_TFUNCTION)
  {
    if(lua_pcall(l, 0, 0, 0))
    {
      printf("error\n%s", lua_tostring(l, -1));
    }
  }
  lua_settop(l, 2);
  printf("loaded opmode %s\n", name);
}

JNIEXPORT void JNICALL start(JNIEnv* env, jobject this, int recognitionId)
{
  lua_getfield(l, -1, "start");
  if(lua_type(l, -1) == LUA_TFUNCTION)
  {
    lua_pushnumber(l, recognitionId);
    if(lua_pcall(l, 1, 0, 0))
    {
      printf("error\n%s", lua_tostring(l, -1));
    }
  }
  lua_settop(l, 2);
}
