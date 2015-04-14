# use this to select gcc instead of clang
NDK_TOOLCHAIN_VERSION := 4.8
# OR use this to select the latest clang version:
#NDK_TOOLCHAIN_VERSION := clang

APP_CPPFLAGS += -std=c++11
APP_STL := gnustl_static

APP_PLATFORM := android-9
APP_ABI := armeabi