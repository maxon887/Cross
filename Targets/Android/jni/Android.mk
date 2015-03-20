LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cross

LOCAL_C_INCLUDES := ../../Sources

LOCAL_SRC_FILES := 	cross.cpp \
					IMP/LauncherIMP.cpp \
					../../../Sources/Graphics.cpp 
					
LOCAL_LDLIBS := -llog -lGLESv1_CM

include $(BUILD_SHARED_LIBRARY)