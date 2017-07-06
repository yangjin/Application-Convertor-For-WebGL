#include "jni.h"
#include "gl_code.h"
#include "gl_impl.h"

static JavaVM * java_vm = NULL;
static jclass c_nativerunnerhelper = NULL;
static jmethodID method_texImage2D = NULL;
static jmethodID method_texSubImage2D = NULL;

static JNIEnv * trace_env;
static jclass c_trace;
static jmethodID method_beginSection;
static jmethodID method_endSection;
static jstring TRACE_TAG;

#define JNITRACE(TAG)     jstring JTAG = trace_env->NewStringUTF(TAG);\
                          trace_env->CallStaticVoidMethod(c_trace, method_beginSection, JTAG);\
                          trace_env->DeleteLocalRef(JTAG);\
                          CHECK_JNI_ERROR(trace_env, method_beginSection, "BeginSection of Trace")

#define JNITRACEEND()  trace_env->CallStaticVoidMethod(c_trace, method_endSection);\
                          CHECK_JNI_ERROR(trace_env, method_endSection, "EndSection of Trace")



bool check_jni_error(JNIEnv* env, bool error, const char * operation_name)
{
    if (error)
    {
        LOGE("Error: %s failed.", operation_name);
        return true;
    }
    else if (env->ExceptionOccurred())
    {
        LOGE("Error: jni exception occurred after %s.", operation_name);
        env->ExceptionDescribe();
        env->ExceptionClear();
        return true;
    }

    #if W2NDEBUG == 1
        if (check_gl_error)
        {
            LOGD("JNI: %s works!", operation_name);
        }
    #endif

    return false;
}

#define CHECK_JNI_ERROR(env, obj, op_name) {if (check_jni_error(env, obj == NULL, op_name)) return;}

jint JNI_OnLoad(JavaVM * vm, void * reserved)
{
    LOGD("JNI: JNI_OnLoad!");

    java_vm = vm;

    JNIEnv * env_;

    if (vm->GetEnv((void**)&env_, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE("Error: GetEnv failed.");
        return -1;
    }

    return JNI_VERSION_1_4;
}

JNIEnv * attach_current_thread()
{
    JNIEnv * env = NULL;
    if (java_vm->AttachCurrentThread(&env, NULL) != JNI_OK)
    {
        return NULL;
    }
    return env;
}

void init_jni_env()
{
    JNIEnv * env_ = attach_current_thread();
    if (!env_) return;

    trace_env = env_;

    //init of Trace
    //android.os.Trace
    c_trace = static_cast<jclass>(env_->NewGlobalRef(env_->FindClass("android/os/Trace")));
    CHECK_JNI_ERROR(env_, c_trace, "FindClass android.os.Trace");

    method_beginSection = env_->GetStaticMethodID(c_trace, "beginSection", "(Ljava/lang/String;)V");
    CHECK_JNI_ERROR(env_, method_beginSection, "GetMethodID of Trace.beginSection(String)");

    method_endSection = env_->GetStaticMethodID(c_trace, "endSection", "()V");
    CHECK_JNI_ERROR(env_, method_endSection, "GetMethodID of Trace.endSection()");

    TRACE_TAG = static_cast<jstring>(env_->NewGlobalRef(env_->NewStringUTF("glTraces")));

    // init of NativeRunnerHelper
    c_nativerunnerhelper = static_cast<jclass>(env_->NewGlobalRef(env_->FindClass("com/android/NativeRunner/Helper")));
    CHECK_JNI_ERROR(env_, c_nativerunnerhelper, "FindClass com.android.NativeRunner.Helper");

    method_texImage2D = env_->GetStaticMethodID(c_nativerunnerhelper, "texImage2D", "(Ljava/lang/String;IIIIII)V");
    CHECK_JNI_ERROR(env_, method_texImage2D, "GetStaticMethodID of Helper.texImage2D");
    method_texSubImage2D = env_->GetStaticMethodID(c_nativerunnerhelper, "texSubImage2D", "(Ljava/lang/String;IIIIIII)V");
    CHECK_JNI_ERROR(env_, method_texSubImage2D, "GetStaticMethodID of Helper.texSubImage2D");
}

// public static void texImage2D(String data, int flipY, int target, int level, int internalformat, int type, int border)
void decodeAndUploadImage(const char* data, int flipY, int target, int level, int internalformat, int type, int border)
{
    JNIEnv * env_ = attach_current_thread();
    if (!env_) return;
    if (internalformat == GL_RGB)
    {
        internalformat = GL_RGBA;
    }
    jstring string_data = env_->NewStringUTF(data);
    CHECK_JNI_ERROR(env_, string_data, "NewStringUTF");

    env_->CallStaticVoidMethod(c_nativerunnerhelper, method_texImage2D, string_data, flipY, target, level, internalformat, type, border);
    CHECK_JNI_ERROR(env_, method_texImage2D, "CallStaticIntMethod of Helper.TexImage2D");
    env_->DeleteLocalRef(string_data);
}

// public static void texSubImage2D(String data, int flipY, int target, int level, int xoffset, int yoffset, int format, int type)
void decodeAndUploadSubImage(const char* data, int flipY, int target, int level, int xoffset, int yoffset, int format, int type)
{
    JNIEnv * env_ = attach_current_thread();
    if (!env_) return;
    jstring string_data = env_->NewStringUTF(data);
    CHECK_JNI_ERROR(env_, string_data, "NewStringUTF");

    env_->CallStaticVoidMethod(c_nativerunnerhelper, method_texSubImage2D, string_data, flipY, target, level, xoffset, yoffset, format, type);
    CHECK_JNI_ERROR(env_, method_texSubImage2D, "CallStaticIntMethod of Helper.texSubImage2D");
    env_->DeleteLocalRef(string_data);
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_NativeRunner_NativeRunnerLib_init(JNIEnv * env, jobject obj,  jint width, jint height,
            jint antialiasingMode, jboolean bCheckGLError, jboolean bEnableProfiling, jint count, jboolean enableDumpTxt);
    JNIEXPORT void JNICALL Java_com_android_NativeRunner_NativeRunnerLib_initGraphics(JNIEnv * env, jobject obj,  jint cWidth, jint cHeight, jint wWidth, jint wHeight);
    JNIEXPORT void JNICALL Java_com_android_NativeRunner_NativeRunnerLib_updateGraphics(JNIEnv * env, jobject obj,  jint cWidth, jint cHeight, jint wWidth, jint wHeight);
    JNIEXPORT void JNICALL Java_com_android_NativeRunner_NativeRunnerLib_step(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT jint JNICALL Java_com_android_NativeRunner_NativeRunnerLib_getDefaultWidth(void);
    JNIEXPORT jint JNICALL Java_com_android_NativeRunner_NativeRunnerLib_getDefaultHeight(void);
    JNIEXPORT jboolean JNICALL Java_com_android_NativeRunner_NativeRunnerLib_getDebugMode(void);
    JNIEXPORT jboolean JNICALL Java_com_android_NativeRunner_NativeRunnerLib_getWebGLAntialiasingConfig(void);
};

JNIEXPORT void JNICALL Java_com_android_NativeRunner_NativeRunnerLib_init(JNIEnv * env, jobject obj, jint width, jint height,
        jint antialiasingMode, jboolean bCheckGLError, jboolean bEnableProfiling, jint count, jboolean enableDumpTxt)
{
    init_jni_env();

    if (antialiasingMode == 0)
    {
        m_antialiasingMode = NoneAA;
    }
    else if (antialiasingMode == 1)
    {
        m_antialiasingMode = MSAAExplicitMode;
    }
    else if (antialiasingMode == 2)
    {
        m_antialiasingMode = MSAAImplicitMode;
    }
    else if (antialiasingMode == 3)
    {
        m_antialiasingMode = CMAAMode;
    }
    check_gl_error = bCheckGLError;
    enable_profiling = bEnableProfiling;
    profiling_count = count;
    initLib(enableDumpTxt);
}

JNIEXPORT void JNICALL Java_com_android_NativeRunner_NativeRunnerLib_initGraphics(JNIEnv * env, jobject obj, jint cWidth, jint cHeight, jint wWidth, jint wHeight)
{
    setupGraphics(cWidth, cHeight, wWidth, wHeight);
}

JNIEXPORT void JNICALL Java_com_android_NativeRunner_NativeRunnerLib_updateGraphics(JNIEnv * env, jobject obj, jint cWidth, jint cHeight, jint wWidth, jint wHeight)
{
    updateGraphics(cWidth, cHeight, wWidth, wHeight);
}

JNIEXPORT void JNICALL Java_com_android_NativeRunner_NativeRunnerLib_step(JNIEnv * env, jobject obj, jint width, jint height)
{
    JNITRACE("RenderFrame");
    renderFrame(width, height);
    JNITRACEEND();
}
JNIEXPORT jint JNICALL Java_com_android_NativeRunner_NativeRunnerLib_getDefaultHeight(void)
{
    return getHeight();
}
JNIEXPORT jint JNICALL Java_com_android_NativeRunner_NativeRunnerLib_getDefaultWidth(void)
{
    return getWidth();
}

JNIEXPORT jboolean JNICALL Java_com_android_NativeRunner_NativeRunnerLib_getDebugMode(void)
{
    return (W2NDEBUG == 1);
}

JNIEXPORT jboolean JNICALL Java_com_android_NativeRunner_NativeRunnerLib_getWebGLAntialiasingConfig(void)
{
    return getAntialias();
}
