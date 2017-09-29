#if !(defined OPENGL)
#include "CrossEGL.h"
#include "Cross.h"
#include "System.h"

using namespace cross;

static const EGLint config_attribs[]{
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
		EGL_STENCIL_SIZE, 8,
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

void CrossEGL::BindWindow(NATIVE_WINDOW wnd) {
    native_window = wnd;
}

NATIVE_WINDOW CrossEGL::GetWindow(){
	return native_window;
}

void CrossEGL::UnbindWindow() {
    if(native_window){
#ifdef ANDROID
        ANativeWindow_release(native_window);
#endif // ANDROID
        native_window = NULL;
    }else{
        system->LogIt("Native window doesn't created");
    }
}

bool CrossEGL::CreateContext(bool createDisplay) {
    system->LogIt("CrossEGL::CreateContext");
    if(context_created){
        system->LogIt("Context already created");
        return false;
    }
    if(createDisplay) {
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            system->LogIt("NativeSurface::CreateDisplay failed. Reason: eglGetDisplay failed.");
            return false;
        }
        if (!eglInitialize(display, 0, 0)) {
            system->LogIt("NativeSurface::CreateDisplay failed. Reason: eglInitialize failed.");
            return false;
        }
        EGLint mathConfigNum;
        if (!eglChooseConfig(display, config_attribs, &config, 1, &mathConfigNum)) {
            system->LogIt("NativeSurface::CreateContext failed. Reason: eglChooseConfig failed.");
            return false;
        }
#ifdef ANDROID
		//new stuff
		EGLint format;
        if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
            system->LogIt("eglGetConfigAttrib() returned error %d", eglGetError());
            return false;
        }
		ANativeWindow_setBuffersGeometry(native_window, 0, 0, format);
#endif // ANDROID
    }
    //create surface
    surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)(native_window), NULL);
    if(surface == EGL_NO_SURFACE){
        system->LogIt("eglCreateWindowSurface failed");
        return false;
    }
    //create context
    if(createDisplay) {
        context = eglCreateContext(display, config, context, context_attribs);
        if (context == EGL_NO_CONTEXT) {
            system->LogIt("eglCreateContext failed");
            return false;
        }
    }

    if(!eglMakeCurrent(display, surface, surface, context)){
        system->LogIt("eglMakeCurrent failed.");
        return false;
    }
    context_created = true;
    return context_created;
}

void CrossEGL::DestroyContext(bool destroyDisplay) {
    system->LogIt("CrossEGL::DestroyContext");
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
        system->LogIt("eglSwapBuffers failed");
    }
}

#endif