#########################################################################
# Build JNI Shared Library
#########################################################################

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS := -Wall -std=c++11 #-Werror -Wno-error=unused-parameter


APP_STL :=stlport_static

LOCAL_C_INCLUDES += ndk/sources/cxx-stl/stlport/stlport
LOCAL_SHARED_LIBRARIES := \
        libcutils \
        liblog \
        libEGL \
        libGLESv2 \
        libstlport\
        libc++\
        libdl\

LOCAL_LDLIBS    := -lc -lm -llog -landroid -lEGL -lGLESv1_CM -lGLESv2  -lGLESv3
LOCAL_MODULE    := nativerunner

LOCAL_SRC_FILES:= \
        global.cpp\
        gl_code.cpp\
        gl_impl.cpp\
        jni_binding.cpp\
        $framesources

include $(BUILD_SHARED_LIBRARY)

