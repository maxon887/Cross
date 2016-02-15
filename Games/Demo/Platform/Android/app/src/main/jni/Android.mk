	LOCAL_PATH := $(call my-dir)
#
# FMOD Shared Library
#
include $(CLEAR_VARS)
LOCAL_MODULE            := fmod
LOCAL_SRC_FILES         := ../lib/$(TARGET_ARCH_ABI)/libfmod.so
LOCAL_EXPORT_C_INCLUDES := FMOD
include $(PREBUILT_SHARED_LIBRARY)

#
# FreeType2 Shared Library
#
include $(CLEAR_VARS)
LOCAL_MODULE            := freetype2-prebuilt
LOCAL_SRC_FILES         := ../lib/$(TARGET_ARCH_ABI)/libfreetype2-static.a
include $(PREBUILT_STATIC_LIBRARY)
#LOCAL_SRC_FILES         := ../lib/$(TARGET_ARCH_ABI)/libfreetype2.so
#include $(PREBUILT_SHARED_LIBRARY)

#
# Cross++ Shared Library
#
include $(CLEAR_VARS)

LOCAL_MODULE := cross
LOCAL_CPP_FEATURES += exceptions
LOCAL_CPP_FEATURES += rtti

LOCAL_C_INCLUDES := ../../../../../../../../Sources/Platform/Android \
					../../../../../../../../Sources/FMOD \
					../../../../../../../../Sources \
					../../../../../../../../Sources/Utils \
					../../../../../../../../Sources/SOIL \
					../../../../../../../../Sources/Shaders \
					../../../../../../../../Sources/FreeType2 \
					../../../../../../../../Sources/FreeType2/freetype2 \
					../../../../../../Sources

LOCAL_SRC_FILES := 	main.cpp \
					../../../../../../../../Sources/Platform/Android/LauncherAndroid.cpp \
					../../../../../../../../Sources/Platform/Android/Cross.cpp \
					../../../../../../../../Sources/Platform/Android/CommercialAndroid.cpp \
					../../../../../../Sources/Demo.cpp \
					../../../../../../Sources/MainScreen.cpp \
					../../../../../../Sources/AnimationScreen.cpp \
					../../../../../../Sources/AudioScreen.cpp \
					../../../../../../Sources/Camera2DScreen.cpp \
					../../../../../../Sources/Menu.cpp \
					../../../../../../Sources/PrimitivesScreen.cpp \
					../../../../../../Sources/SpritesScreen.cpp \
					../../../../../../Sources/TestNaPidoraScreen.cpp \
					../../../../../../Sources/TextScreen.cpp \
					../../../../../../../../Sources/Graphics2D.cpp \
					../../../../../../../../Sources/Camera.cpp \
					../../../../../../../../Sources/Exception.cpp \
					../../../../../../../../Sources/File.cpp \
					../../../../../../../../Sources/Font.cpp \
					../../../../../../../../Sources/GraphicsGL.cpp \
					../../../../../../../../Sources/Matrix.cpp \
					../../../../../../../../Sources/MemoryManager.cpp \
					../../../../../../../../Sources/Vector2D.cpp \
					../../../../../../../../Sources/Vector3D.cpp \
					../../../../../../../../Sources/Vector4D.cpp \
					../../../../../../../../Sources/Audio.cpp \
					../../../../../../../../Sources/Game.cpp \
					../../../../../../../../Sources/Launcher.cpp \
					../../../../../../../../Sources/Sprite.cpp \
					../../../../../../../../Sources/Input.cpp \
					../../../../../../../../Sources/Config.cpp \
					../../../../../../../../Sources/Rect.cpp \
					../../../../../../../../Sources/Utils/Texter.cpp \
					../../../../../../../../Sources/Color.cpp \
					../../../../../../../../Sources/Utils/Debuger.cpp \
					../../../../../../../../Sources/Utils/Button.cpp \
					../../../../../../../../Sources/Utils/Animation.cpp \
					../../../../../../../../Sources/Utils/ToggleButton.cpp \
					../../../../../../../../Sources/Utils/Commercial.cpp \
					../../../../../../../../Sources/Utils/Misc.cpp \
					../../../../../../../../Sources/Shaders/PrimitiveShaders.cpp \
					../../../../../../../../Sources/Shaders/SpriteShaders.cpp \
					../../../../../../../../Sources/SOIL/SOIL.c \
					../../../../../../../../Sources/SOIL/image_helper.c \
					../../../../../../../../Sources/SOIL/stb_image_aug.c

LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2
LOCAL_SHARED_LIBRARIES  := fmod
LOCAL_STATIC_LIBRARIES  := freetype2-prebuilt

include $(BUILD_SHARED_LIBRARY)