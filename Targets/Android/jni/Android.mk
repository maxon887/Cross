LOCAL_PATH := $(call my-dir)
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
					../Android/LauncherAndroid.cpp \
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
					../../../Sources/Utils/PointX.cpp \
					../../../Sources/Utils/RectX.cpp \
					../../../Sources/Utils/Texter.cpp \
					../../../Sources/Utils/Debuger.cpp \
					../../../Sources/Utils/Button.cpp \
					../../../Sources/Utils/Animation.cpp \
					../../../Sources/SOIL/SOIL.c \
					../../../Sources/SOIL/image_helper.c \
					../../../Sources/SOIL/stb_image_aug.c 
					
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)