#pragma once
#include <EGL/egl.h>

#ifdef WIN
#define NATIVE_WINDOW HWND
#elif ANDROID
#include <android/native_window.h>
#define NATIVE_WINDOW ANativeWindow*
#endif

class CrossEGL {
public:
    CrossEGL();

    void BindWindow(NATIVE_WINDOW wnd);
	NATIVE_WINDOW GetWindow();
    void UnbindWindow();
    bool CreateContext(bool createDisplay);
    void DestroyContext(bool destroyDisplay);
    bool IsContextCreated();

    void SwapBuffers();

private:
    NATIVE_WINDOW  native_window;

    bool        context_created;
    EGLConfig   config;
    EGLDisplay  display;
    EGLSurface  surface;
    EGLContext  context;
};