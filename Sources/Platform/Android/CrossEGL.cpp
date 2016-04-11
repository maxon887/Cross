#include "CrossEGL.h"
#include "LauncherAndroid.h"

#include <string.h>

static const EGLint config_attribs[]{
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE, 4,
        EGL_GREEN_SIZE, 4,
        EGL_BLUE_SIZE, 4,
        EGL_DEPTH_SIZE, 4,
        EGL_NONE,
};

static const EGLint context_attribs[] {
                EGL_CONTEXT_CLIENT_VERSION,  2,
                EGL_NONE
        };

CrossEGL::CrossEGL() :
        native_window(0),
        context_created(false),
        config(0),
        display(EGL_NO_DISPLAY),
        surface(EGL_NO_SURFACE),
        context(EGL_NO_CONTEXT)
{ }

void CrossEGL::BindWindow(ANativeWindow *wnd) {
    native_window = wnd;
}

void CrossEGL::UnbindWindow() {
    if(native_window){
        ANativeWindow_release(native_window);
        native_window = NULL;
    }else{
        LOGE("Native window doesn't created");
    }
}

bool CrossEGL::CreateContext(bool createDisplay) {
    LOGI("CrossEGL::CreateContext");
    if(context_created){
        LOGE("Context already created");
        return false;
    }
    if(createDisplay) {
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            LOGE("NativeSurface::CreateDisplay failed. Reason: eglGetDisplay failed.");
            return false;
        }
        if (!eglInitialize(display, 0, 0)) {
            LOGE("NativeSurface::CreateDisplay failed. Reason: eglInitialize failed.");
            return false;
        }
        EGLint mathConfigNum;
        if (!eglChooseConfig(display, config_attribs, &config, 1, &mathConfigNum)) {
            LOGE("NativeSurface::CreateContext failed. Reason: eglChooseConfig failed.");
            return false;
        }
        //new stuff
        if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
            LOGE("eglGetConfigAttrib() returned error %d", eglGetError());
            return false;
        }

        ANativeWindow_setBuffersGeometry(native_window, 0, 0, format);
    }
    //create surface
    surface = eglCreateWindowSurface(display, config, native_window, NULL);
    if(surface == EGL_NO_SURFACE){
        LOGE("eglCreateWindowSurface failed");
        return false;
    }
    //create context
    if(createDisplay) {
        context = eglCreateContext(display, config, context, context_attribs);
        if (context == EGL_NO_CONTEXT) {
            LOGE("eglCreateContext failed");
            return false;
        }
    }

    if(!eglMakeCurrent(display, surface, surface, context)){
        LOGE("eglMakeCurrent failed.");
        return false;
    }
    context_created = true;
    return context_created;
}

void CrossEGL::DestroyContext(bool destroyDisplay) {
    LOGI("CrossEGL::DestroyContext");
    if(display){
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if(destroyDisplay){
            if(context != EGL_NO_CONTEXT){
                eglDestroyContext(display, context);
                context = EGL_NO_CONTEXT;
            }
        }

        if(surface != EGL_NO_SURFACE){
            eglDestroySurface(display, surface);
            surface = EGL_NO_SURFACE;
        }
        if(destroyDisplay){
            eglTerminate(display);
            display = EGL_NO_DISPLAY;
        }
    }
    context_created = false;
}

bool CrossEGL::IsContextCreated() {
    return context_created;
}

void CrossEGL::SwapBuffers() {
    if(!eglSwapBuffers(display, surface)){
        LOGE("eglSwapBuffers failed");
    }
}