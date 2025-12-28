#ifndef PAL_ANDROID_H
#define PAL_ANDROID_H

#include <jni.h>
#include "pal.h"

/* Initialize JNI environment for PAL callbacks */
void pal_android_set_jni_env(JavaVM *jvm, jobject interpreter_obj, JNIEnv *env);

/* Cleanup JNI resources */
void pal_android_cleanup(JNIEnv *env);

/* Submit input from Java layer */
void pal_android_submit_input(const char *input);

/* PAL instance */
extern basic_pal_t pal_android;

#endif
