LOCAL_PATH := $(call my-dir)
#
# FMOD Shared Library
# 
include $(CLEAR_VARS)

LOCAL_MODULE            := fmod
LOCAL_SRC_FILES         := ../FMOD/libfmodL.so
LOCAL_EXPORT_C_INCLUDES := FMOD

include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := cross
LOCAL_CPP_FEATURES += exceptions

LOCAL_C_INCLUDES := Android \
					../../Sources \
					../../Sources/Utils \
					../../Sources/SOIL \
					../../Games/Snaky/Sources \
					../../Games/Demo/Sources 

LOCAL_SRC_FILES := 	cross.cpp \
					../FMOD/common.cpp \
					../FMOD/common_platform.cpp \
					../Android/LauncherAndroid.cpp \
					../Android/AudioAndroid.cpp \
					../../../Games/Snaky/Sources/SnakyGame.cpp \
					../../../Games/Snaky/Sources/MenuScreen.cpp \
					../../../Games/Demo/Sources/Demo.cpp \
					../../../Games/Demo/Sources/MainScreen.cpp \
					../../../Games/Demo/Sources/SecondScreen.cpp \
					../../../Sources/Graphics.cpp \
					../../../Sources/Game.cpp \
					../../../Sources/Screen.cpp \
					../../../Sources/Image.cpp \
					../../../Sources/Input.cpp \
					../../../Sources/Saver.cpp \
					../../../Sources/Utils/PointX.cpp \
					../../../Sources/Utils/RectX.cpp \
					../../../Sources/Utils/Texter.cpp \
					../../../Sources/Utils/Debuger.cpp \
					../../../Sources/Utils/Button.cpp \
					../../../Sources/Utils/Animation.cpp \
					../../../Sources/Utils/ToggleButton.cpp \
					../../../Sources/SOIL/SOIL.c \
					../../../Sources/SOIL/image_helper.c \
					../../../Sources/SOIL/stb_image_aug.c 
					
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv1_CM -lOpenSLES
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_SHARED_LIBRARIES  := fmod

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)