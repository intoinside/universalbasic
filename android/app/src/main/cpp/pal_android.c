#include <jni.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "pal.h"

/* Global JNI state */
static JavaVM *g_jvm = NULL;
static jobject g_interpreter_obj = NULL;
static jmethodID g_append_output_method = NULL;
static jmethodID g_append_error_method = NULL;
static jmethodID g_on_input_requested_method = NULL;

/* Threading primitives for input synchronization */
static pthread_mutex_t input_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t input_cond = PTHREAD_COND_INITIALIZER;
static char input_buffer[256];
static int input_ready = 0;

/* Initialize JNI references - called from native-lib.c */
void pal_android_set_jni_env(JavaVM *jvm, jobject interpreter_obj, JNIEnv *env) {
    g_jvm = jvm;
    
    if (g_interpreter_obj) {
        (*env)->DeleteGlobalRef(env, g_interpreter_obj);
    }
    g_interpreter_obj = (*env)->NewGlobalRef(env, interpreter_obj);
    
    /* Get method IDs */
    jclass clazz = (*env)->GetObjectClass(env, interpreter_obj);
    g_append_output_method = (*env)->GetMethodID(env, clazz, "appendOutput", "(Ljava/lang/String;)V");
    g_append_error_method = (*env)->GetMethodID(env, clazz, "appendError", "(Ljava/lang/String;)V");
    g_on_input_requested_method = (*env)->GetMethodID(env, clazz, "onInputRequested", "()V");
    (*env)->DeleteLocalRef(env, clazz);
}

/* Cleanup JNI references */
void pal_android_cleanup(JNIEnv *env) {
    if (g_interpreter_obj) {
        (*env)->DeleteGlobalRef(env, g_interpreter_obj);
        g_interpreter_obj = NULL;
    }
}

/* Helper to get JNIEnv for current thread */
static JNIEnv* get_jni_env() {
    JNIEnv *env = NULL;
    if (!g_jvm) return NULL;
    
    jint status = (*g_jvm)->GetEnv(g_jvm, (void**)&env, JNI_VERSION_1_6);
    
    if (status == JNI_EDETACHED) {
        if ((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK) {
            return NULL;
        }
    }
    
    return env;
}

/* PAL Implementation */
static void android_print(const char *s) {
    if (!s || !g_interpreter_obj || !g_append_output_method) return;
    
    JNIEnv *env = get_jni_env();
    if (!env) return;
    
    jstring jstr = (*env)->NewStringUTF(env, s);
    (*env)->CallVoidMethod(env, g_interpreter_obj, g_append_output_method, jstr);
    (*env)->DeleteLocalRef(env, jstr);
}

static void android_print_error(const char *s) {
    if (!s || !g_interpreter_obj || !g_append_error_method) return;
    
    JNIEnv *env = get_jni_env();
    if (!env) return;
    
    jstring jstr = (*env)->NewStringUTF(env, s);
    (*env)->CallVoidMethod(env, g_interpreter_obj, g_append_error_method, jstr);
    (*env)->DeleteLocalRef(env, jstr);
}

static void android_newline(void) {
    android_print("\n");
}

static int android_get_char(void) {
    /* Not used in Android - we use line-based input */
    return -1;
}

static char *android_input_line(char *buffer, int size) {
    if (!g_interpreter_obj || !g_on_input_requested_method) return NULL;
    
    JNIEnv *env = get_jni_env();
    if (!env) return NULL;
    
    /* Notify Java that input is needed */
    (*env)->CallVoidMethod(env, g_interpreter_obj, g_on_input_requested_method);
    
    /* Wait for input to be submitted from Java thread */
    pthread_mutex_lock(&input_mutex);
    input_ready = 0;
    
    while (!input_ready) {
        pthread_cond_wait(&input_cond, &input_mutex);
    }
    
    /* Copy input to caller's buffer */
    strncpy(buffer, input_buffer, size - 1);
    buffer[size - 1] = '\0';
    
    pthread_mutex_unlock(&input_mutex);
    
    return buffer;
}

/* Called from Java when user submits input */
void pal_android_submit_input(const char *input) {
    pthread_mutex_lock(&input_mutex);
    
    strncpy(input_buffer, input, sizeof(input_buffer) - 1);
    input_buffer[sizeof(input_buffer) - 1] = '\0';
    input_ready = 1;
    
    pthread_cond_signal(&input_cond);
    pthread_mutex_unlock(&input_mutex);
}

#include <unistd.h>

static void android_yield(void) {
    /* Throttling for Android to save battery and prevent CPU hogging */
    usleep(100); 
}

/* PAL structure */
basic_pal_t pal_android = {
    .print = android_print,
    .print_error = android_print_error,
    .newline = android_newline,
    .get_char = android_get_char,
    .input_line = android_input_line,
    .yield = android_yield
};
