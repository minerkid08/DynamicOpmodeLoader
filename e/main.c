#include <jni.h>

#define getString Java_com_minerkid08_dynamicopmodeloader_NativeLib_stringFromJNI

JNIEXPORT jstring JNICALL getString(JNIEnv *env, jobject this) {
  const char *str = "did dis work";
  return (*env)->NewStringUTF(env, str);
}
