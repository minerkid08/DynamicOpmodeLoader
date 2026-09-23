#include "opmodeManager.h"
#include "dynList.h"
#include "jni.h"
#include "utils.h"

static jclass class = 0;
static jmethodID constructor;
static jfieldID nameField;
static jfieldID typeField;
static jfieldID groupField;
static jfieldID orderField;

void initOpmodeClass(JNIEnv* env)
{
	if (class == 0)
	{
		class = (*env)->FindClass(env, "com/minerkid08/dynamicopmodeloader/Opmode");
		constructor = (*env)->GetMethodID(env, class, "<init>", "()V");
		nameField = (*env)->GetFieldID(env, class, "name", "Ljava/lang/String;");
		typeField = (*env)->GetFieldID(env, class, "type", "I");
		groupField = (*env)->GetFieldID(env, class, "group", "Ljava/lang/String;");
		orderField = (*env)->GetFieldID(env, class, "order", "Ljava/lang/String;");
		class = (*env)->NewGlobalRef(env, class);
	}
}

jobjectArray genOpmodeArray(JNIEnv* env, Opmode* opmodes)
{
  initOpmodeClass(env);
	int opmodeCount = dynList_size(opmodes);

	jobjectArray arr = (*env)->NewObjectArray(env, opmodeCount, class, NULL);

	print("loaded %d opmodes\n", opmodeCount);

	for (int i = 0; i < opmodeCount; i++)
	{
		Opmode* opmode = opmodes + i;
		jobject object = (*env)->NewObject(env, class, constructor);

		jstring str = (*env)->NewStringUTF(env, opmode->name);
		(*env)->SetObjectField(env, object, nameField, str);
		(*env)->DeleteLocalRef(env, str);

		(*env)->SetIntField(env, object, typeField, opmode->type);

		if (opmode->group)
		{
			str = (*env)->NewStringUTF(env, opmode->group);
			(*env)->SetObjectField(env, object, groupField, str);
			(*env)->DeleteLocalRef(env, str);
		}

		if (opmode->order)
		{
			str = (*env)->NewStringUTF(env, opmode->order);
			(*env)->SetObjectField(env, object, orderField, str);
			(*env)->DeleteLocalRef(env, str);
		}

		(*env)->SetObjectArrayElement(env, arr, i, object);
	}
	return arr;
}
