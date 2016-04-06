#pragma once
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/native_window.h>

class CrossEGL {
public:
    CrossEGL();

    void BindWindow(ANativeWindow* wnd);
    void UnbindWindow();
    bool CreateContext(bool createDisplay);
    void DestroyContext(bool destroyDisplay);
    bool IsContextCreated();

    void SwapBuffers();

private:
    ANativeWindow*  native_window;

    bool        context_created;
    EGLConfig   config;
    EGLDisplay  display;
    EGLSurface  surface;
    EGLContext  context;
    EGLint      format;
};