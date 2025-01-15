#include "functionBuilder.h"
#include "dynList.h"
#include "function.h"
#include "global.h"
#include "jni.h"
#include "jni_md.h"
#include <string.h>

#define addObject Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_setCurrentObject
#define addFun Java_com_minerkid08_dynamicopmodeloader_FunctionBuilder_addFunction

void initFunctionBuilder()
{
	global.objects = dynList_new(5, sizeof(jobject));
	dynList_resize((void**)&global.objects, 0);

	global.functions = dynList_new(10, sizeof(Function));
	dynList_resize((void**)&global.functions, 0);
}

void reset()
{
}

JNIEXPORT void JNICALL addObject(JNIEnv* env, jobject this, jobject object)
{
	global.env = env;

	jobject obj = (*env)->NewGlobalRef(env, object);
	global.currentObject = obj;

	int objectId = dynList_size(global.objects);
	dynList_resize((void**)&global.objects, objectId + 1);
	*(global.objects + objectId) = obj;
}

JNIEXPORT void JNICALL addFun(JNIEnv* env, jobject this, jstring name, jstring signature, int rtnType, int argc)
{
  const char* name2 = (*env)->GetStringUTFChars(env, name, NULL);
  const char* signature2 = (*env)->GetStringUTFChars(env, signature, NULL);

  global.env = env;

	int functionId = dynList_size(global.functions);
	dynList_resize((void**)&global.functions, functionId + 1);
	Function* fun = global.functions + functionId;
  function_init(fun, name2, signature2, rtnType, argc);
}
