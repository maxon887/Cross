LOCAL_PATH := $(call my-dir)
#
# FMOD Shared Library
# 
include $(CLEAR_VARS)

LOCAL_MODULE            := fmod
LOCAL_SRC_FILES         := ../lib/$(TARGET_ARCH_ABI)/libfmod.so
LOCAL_EXPORT_C_INCLUDES := FMOD

include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := cross
LOCAL_CPP_FEATURES += exceptions
LOCAL_CPP_FEATURES += rtti
LOCAL_CFLAGS    := -DC3D

LOCAL_C_INCLUDES := ../../../../Sources/Platform/Android \
					../../../../Sources/FMOD \
					../../../../Sources \
					../../../../Sources/Utils \
					../../../../Sources/SOIL \
					../../Sources 

LOCAL_SRC_FILES := 	main.cpp \
					../../../../../Sources/Platform/Android/LauncherAndroid.cpp \
					../../../../../Sources/Platform/Android/Cross.cpp \
					../../../../../Sources/Platform/Android/CommercialAndroid.cpp \
					../../../Sources/Cross3D.cpp \
					../../../Sources/Graphics3D.cpp \
					../../../Sources/MainScreen.cpp \
					../../../Sources/HelloTriangleScreen.cpp \
					../../../../../Sources/Audio.cpp \
					../../../../../Sources/Game.cpp \
					../../../../../Sources/Image.cpp \
					../../../../../Sources/Input.cpp \
					../../../../../Sources/Saver.cpp \
					../../../../../Sources/Point.cpp \
					../../../../../Sources/Rect.cpp \
					../../../../../Sources/Color.cpp \
					../../../../../Sources/Utils/Debuger.cpp \
					../../../../../Sources/Utils/Commercial.cpp \
					../../../../../Sources/SOIL/SOIL.c \
					../../../../../Sources/SOIL/image_helper.c \
					../../../../../Sources/SOIL/stb_image_aug.c 
					
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2
LOCAL_SHARED_LIBRARIES  := fmod

include $(BUILD_SHARED_LIBRARY)