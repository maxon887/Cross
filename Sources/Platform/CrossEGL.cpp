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
		native_window = null;
	}else{
		system->LogIt("Native window doesn't created");
	}
}

bool CrossEGL::CreateContext(bool createDisplay) {
	system->LogIt("CrossEGL::CreateContext");
	CROSS_RETURN(!context_created, false, "Context already created");
	if(createDisplay) {
		display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		CROSS_RETURN(display != EGL_NO_DISPLAY, false, "NativeSurface::CreateDisplay failed. Reason: eglGetDisplay failed.");
		CROSS_RETURN(eglInitialize(display, 0, 0), false, "NativeSurface::CreateDisplay failed. Reason: eglInitialize failed.");
		EGLint mathConfigNum;
		CROSS_RETURN(eglChooseConfig(display, config_attribs, &config, 1, &mathConfigNum), false,
			"NativeSurface::CreateContext failed. Reason: eglChooseConfig failed.");
#ifdef ANDROID
		//new stuff
		EGLint format;
		CROSS_RETURN(eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format), false, 
			"eglGetConfigAttrib() returned error %d", eglGetError());
		ANativeWindow_setBuffersGeometry(native_window, 0, 0, format);
#endif // ANDROID
	}
	//create surface
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)(native_window), null);
	CROSS_RETURN(surface != EGL_NO_SURFACE, false, "eglCreateWindowSurface failed");
	//create context
	if(createDisplay) {
		context = eglCreateContext(display, config, context, context_attribs);
		CROSS_RETURN(context != EGL_NO_CONTEXT, false, "eglCreateContext failed");
	}

	CROSS_RETURN(eglMakeCurrent(display, surface, surface, context), false, "eglMakeCurrent failed.");
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