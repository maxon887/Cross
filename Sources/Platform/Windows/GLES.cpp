/*	Copyright © 2015 Lukyanau Maksim

	This file is part of Cross++ Game Engine.

    Cross++ Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cross++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#ifdef GLES
#include "GLES.h"
#include "Native.h"
#include "Game.h"
#include "Screen.h"
#include "LauncherWIN.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"
#include "Config.h"

using namespace cross;

Game* cross::game = NULL;

int GLES_GO(){

	ESContext esContext;
	memset(&esContext, 0, sizeof(ESContext));
	
	esContext.hWnd = WinCreate();

	try{
		srand((unsigned int)time(0));
		LauncherWIN launcherWin(esContext.hWnd);
		launcher = &launcherWin;
		game = CrossMain(launcher);
		input->KeyReleased += MakeDelegate(&launcherWin, &LauncherWIN::KeyReleasedHandle);

		int winX = config->LoadInt("WIN_POS_X", 0);
		int winY = config->LoadInt("WIN_POS_Y", 0);
		int winWidth = config->LoadInt("WIN_WIDTH", 500);
		int winHeight = config->LoadInt("WIN_HEIGHT", 500);
		launcherWin.ResizeWindow(winX, winY, winWidth, winHeight);

		esCreateWindow(&esContext, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

		ShowWindow(esContext.hWnd, TRUE);

		gfxGL = new GraphicsGL();
		gfx2D = new Graphics2D();
		game->Start();
		game->SetScreen(game->GetStartScreen());

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));
		while(msg.message != WM_QUIT) {
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			game->Update();
			eglSwapBuffers(esContext.eglDisplay, esContext.eglSurface);
		}

		game->GetCurrentScreen()->Stop();
		game->Stop();
		delete gfx2D;
		delete gfxGL;
		delete game;

		unsigned long leaked = MemoryManager::Instance()->Dump();
		if(leaked > 0) {
			char buf[256];
			sprintf(buf, "Memory leak.Total bytes = %d\n", leaked);
			OutputDebugString(buf);
			return -1;
		} else {
			OutputDebugString("No memory leak detected\n");
		}

	} catch(Exception &exc) {
		string msg = string(exc.message) +
			+"\nFile: " + string(exc.filename) +
			+"\nLine: " + to_string(exc.line);
		OutputDebugString(msg.c_str());
		MessageBox(esContext.hWnd, msg.c_str(), "Unhandled Exception", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}
	return 0;
}

// CreateEGLContext()
//Creates an EGL rendering context and all associated elements
EGLBoolean CreateEGLContext(EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
	EGLContext* eglContext, EGLSurface* eglSurface,
	EGLint attribList[])
{
	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	// Get Display
	display = eglGetDisplay(GetDC(hWnd));
	if(display == EGL_NO_DISPLAY)	{
		return EGL_FALSE;
	}

	// Initialize EGL
	if(!eglInitialize(display, &majorVersion, &minorVersion))	{
		return EGL_FALSE;
	}

	// Get configs
	if(!eglGetConfigs(display, NULL, 0, &numConfigs))	{
		return EGL_FALSE;
	}

	// Choose config
	if(!eglChooseConfig(display, attribList, &config, 1, &numConfigs))	{
		return EGL_FALSE;
	}

	// Create a surface
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
	if(surface == EGL_NO_SURFACE)	{
		return EGL_FALSE;
	}

	// Create a GL context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if(context == EGL_NO_CONTEXT)	{
		return EGL_FALSE;
	}

	// Make the context current
	if(!eglMakeCurrent(display, surface, surface, context))	{
		return EGL_FALSE;
	}

	*eglDisplay = display;
	*eglSurface = surface;
	*eglContext = context;
	return EGL_TRUE;
}


//  esCreateWindow()
//
//      flags  - bitwise or of window creation flags 
//          ES_WINDOW_ALPHA       - specifies that the framebuffer should have alpha
//          ES_WINDOW_DEPTH       - specifies that a depth buffer should be created
//          ES_WINDOW_STENCIL     - specifies that a stencil buffer should be created
//          ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
GLboolean esCreateWindow(ESContext *esContext, GLuint flags)
{
	EGLint attribList[] =
	{
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 5,
		EGL_ALPHA_SIZE, (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE, (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
		EGL_STENCIL_SIZE, (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
		EGL_NONE
	};

	if(esContext == NULL)
	{
		return GL_FALSE;
	}

	if(!CreateEGLContext(esContext->hWnd,
		&esContext->eglDisplay,
		&esContext->eglContext,
		&esContext->eglSurface,
		attribList))
	{
		return GL_FALSE;
	}

	return GL_TRUE;
}


#endif