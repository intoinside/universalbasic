#include <jni.h>
#include <pthread.h>
#include <string.h>
#include "basic.h"
#include "pal_android.h"

/* Global interpreter state */
static BasicState g_state;
static pthread_t interpreter_thread;
static int interpreter_initialized = 0;
static JavaVM *g_jvm = NULL;

/* Thread function for running commands */
typedef struct {
    char command[256];
    int execute;
} CommandTask;

static pthread_mutex_t task_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t task_cond = PTHREAD_COND_INITIALIZER;
static CommandTask current_task = {0};

static void* interpreter_thread_func(void* arg) {
    (void)arg;
    char command_to_run[256];
    
    JNIEnv *env = NULL;
    if (g_jvm) {
        (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);
    }
    
    while (1) {
        pthread_mutex_lock(&task_mutex);
        
        while (!current_task.execute) {
            pthread_cond_wait(&task_cond, &task_mutex);
        }
        
        if (strcmp(current_task.command, "__EXIT__") == 0) {
            pthread_mutex_unlock(&task_mutex);
            break;
        }
        
        /* Copy command and release mutex to avoid deadlock during slow execution or INPUT */
        strncpy(command_to_run, current_task.command, sizeof(command_to_run) - 1);
        command_to_run[sizeof(command_to_run) - 1] = '\0';
        current_task.execute = 0;
        
        pthread_mutex_unlock(&task_mutex);
        
        /* Execute the command */
        basic_eval_line(&g_state, command_to_run);
        
        /* Print prompt after execution */
        g_state.pal->print("READY.\n");
    }
    
    if (g_jvm) {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }
    return NULL;
}

JNIEXPORT void JNICALL
Java_com_universalbasic_BasicInterpreter_nativeInit(JNIEnv *env, jobject thiz) {
    if (interpreter_initialized) {
        return;
    }
    
    (*env)->GetJavaVM(env, &g_jvm);
    
    /* Initialize PAL with JNI environment */
    pal_android_set_jni_env(g_jvm, thiz, env);
    
    /* Initialize BASIC interpreter */
    basic_init(&g_state, &pal_android);
    
    /* Create interpreter thread */
    pthread_create(&interpreter_thread, NULL, interpreter_thread_func, NULL);
    
    interpreter_initialized = 1;
    
    /* Send welcome message */
    g_state.pal->print("UNIVERSAL BASIC 2.0\n");
    g_state.pal->print("Version: ");
    g_state.pal->print(APP_VERSION);
    g_state.pal->print("\nReady.\n");
}

JNIEXPORT void JNICALL
Java_com_universalbasic_BasicInterpreter_nativeEvalLine(JNIEnv *env, jobject thiz, jstring line) {
    if (!interpreter_initialized) {
        return;
    }
    
    const char *native_line = (*env)->GetStringUTFChars(env, line, NULL);
    
    pthread_mutex_lock(&task_mutex);
    strncpy(current_task.command, native_line, sizeof(current_task.command) - 1);
    current_task.command[sizeof(current_task.command) - 1] = '\0';
    current_task.execute = 1;
    pthread_cond_signal(&task_cond);
    pthread_mutex_unlock(&task_mutex);
    
    (*env)->ReleaseStringUTFChars(env, line, native_line);
}

JNIEXPORT void JNICALL
Java_com_universalbasic_BasicInterpreter_nativeSubmitInput(JNIEnv *env, jobject thiz, jstring input) {
    const char *native_input = (*env)->GetStringUTFChars(env, input, NULL);
    pal_android_submit_input(native_input);
    (*env)->ReleaseStringUTFChars(env, input, native_input);
}

JNIEXPORT void JNICALL
Java_com_universalbasic_BasicInterpreter_nativeStop(JNIEnv *env, jobject thiz) {
    g_state.should_exit = 1;
}

JNIEXPORT void JNICALL
Java_com_universalbasic_BasicInterpreter_nativeShutdown(JNIEnv *env, jobject thiz) {
    if (!interpreter_initialized) {
        return;
    }
    
    /* Signal thread to exit */
    pthread_mutex_lock(&task_mutex);
    strcpy(current_task.command, "__EXIT__");
    current_task.execute = 1;
    pthread_cond_signal(&task_cond);
    pthread_mutex_unlock(&task_mutex);
    
    /* Wait for thread to finish */
    pthread_join(interpreter_thread, NULL);
    
    /* Cleanup */
    pal_android_cleanup(env);
    interpreter_initialized = 0;
}
