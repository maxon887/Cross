LOCAL_PATH := $(call my-dir)
#
# FMOD Shared Library
# 
include $(CLEAR_VARS)

LOCAL_MODULE            := fmod
LOCAL_SRC_FILES         := ../libfmodL.so
LOCAL_EXPORT_C_INCLUDES := FMOD

include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := cross
LOCAL_CPP_FEATURES += exceptions
LOCAL_CPP_FEATURES += rtti

LOCAL_C_INCLUDES := ../../../../Sources/Platform/Android \
					../../../../Sources/FMOD \
					../../../../Sources \
					../../../../Sources/Utils \
					../../../../Sources/SOIL \
					../../Sources 

LOCAL_SRC_FILES := 	main.cpp \
					../../../../../Sources/Platform/Android/LauncherAndroid.cpp \
					../../../../../Sources/Platform/Android/Cross.cpp \
					../../../Sources/MenuScreen.cpp \
					../../../Sources/JimTheSnake.cpp \
					../../../Sources/GameScreen.cpp \
					../../../Sources/Apple.cpp \
					../../../Sources/Spider.cpp \
					../../../Sources/Snake.cpp \
					../../../../../Sources/Graphics.cpp \
					../../../../../Sources/Game.cpp \
					../../../../../Sources/Screen.cpp \
					../../../../../Sources/Image.cpp \
					../../../../../Sources/Input.cpp \
					../../../../../Sources/Saver.cpp \
					../../../../../Sources/Audio.cpp \
					../../../../../Sources/PointX.cpp \
					../../../../../Sources/RectX.cpp \
					../../../../../Sources/Utils/Texter.cpp \
					../../../../../Sources/Utils/Debuger.cpp \
					../../../../../Sources/Utils/Button.cpp \
					../../../../../Sources/Utils/Animation.cpp \
					../../../../../Sources/Utils/ToggleButton.cpp \
					../../../../../Sources/ColorX.cpp \
					../../../../../Sources/Utils/Misc.cpp \
					../../../../../Sources/SOIL/SOIL.c \
					../../../../../Sources/SOIL/image_helper.c \
					../../../../../Sources/SOIL/stb_image_aug.c 
					
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv1_CM -lOpenSLES
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_SHARED_LIBRARIES  := fmod

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)