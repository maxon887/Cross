# use this to select gcc instead of clang
NDK_TOOLCHAIN_VERSION := 4.8

APP_CPPFLAGS += -std=c++11
APP_STL := c++_shared

APP_PLATFORM := android-9
APP_ABI := armeabi