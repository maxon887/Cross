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
#include "GLES.h"
#include "Game.h"
#include "Screen.h"
#include "LauncherWIN.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"

using namespace cross;

Game* cross::game = NULL;

void Draw(ESContext *esContext)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//SceneManager::Instance()->Draw();
	game->Update();
	//gfx2D->DrawCircle(Vector2D(50, 50), 20, Color::Red);

	//eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
	//game->Update();

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext *esContext, float deltaTime)
{
	//game->Update();
	//game->Update();
}

void KeyHandler(ESContext *esContext, unsigned char key, bool bIsPressed)
{
	//Globals::input.pressedKeys[key] = bIsPressed;
}

int GLES_GO(){

	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "Cross++", 512, 512, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
	
	try{
		LauncherWIN launcherWin(esContext.hWnd);
		launcher = &launcherWin;
		game = CrossMain(launcher);

		esRegisterDrawFunc(&esContext, Draw);
		esRegisterUpdateFunc(&esContext, Update);
		esRegisterKeyFunc(&esContext, KeyHandler);

		gfxGL = new GraphicsGL();
		gfx2D = new Graphics2D();
		game->Start();
		game->Init();

		esMainLoop(&esContext);

		game->GetCurrentScreen()->Stop();
		game->Stop();
		delete game;
		delete gfx2D;
		delete gfxGL;

		unsigned long leaked = MemoryManager::Instance()->Dump();
		if(leaked > 0){
			launcher->LogIt("Memory leak.Total bytes = %d\n", leaked);
		} else{
			launcher->LogIt("No memory leak detected\n");
		}
	} catch(Exception &exc) {
		string msg = string(exc.message) +
			+"\nFile: " + string(exc.filename) +
			+"\nLine: " + to_string(exc.line);
		OutputDebugString(msg.c_str());
		MessageBox(esContext.hWnd, msg.c_str(), "Unhandled Exception", MB_OK | MB_ICONEXCLAMATION);
		return 0;
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
	if(display == EGL_NO_DISPLAY)
	{
		return EGL_FALSE;
	}

	// Initialize EGL
	if(!eglInitialize(display, &majorVersion, &minorVersion))
	{
		return EGL_FALSE;
	}

	// Get configs
	if(!eglGetConfigs(display, NULL, 0, &numConfigs))
	{
		return EGL_FALSE;
	}

	// Choose config
	if(!eglChooseConfig(display, attribList, &config, 1, &numConfigs))
	{
		return EGL_FALSE;
	}

	// Create a surface
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
	if(surface == EGL_NO_SURFACE)
	{
		return EGL_FALSE;
	}

	// Create a GL context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if(context == EGL_NO_CONTEXT)
	{
		return EGL_FALSE;
	}

	// Make the context current
	if(!eglMakeCurrent(display, surface, surface, context))
	{
		return EGL_FALSE;
	}

	*eglDisplay = display;
	*eglSurface = surface;
	*eglContext = context;
	return EGL_TRUE;
}

//  esInitContext()
//
//      Initialize ES utility context.  This must be called before calling any other
//      functions.

void ESUTIL_API esInitContext(ESContext *esContext)
{
	if(esContext != NULL)
	{
		memset(esContext, 0, sizeof(ESContext));
	}
}


//  esCreateWindow()
//
//      title - name for title bar of window
//      width - width of window to create
//      height - height of window to create
//      flags  - bitwise or of window creation flags 
//          ES_WINDOW_ALPHA       - specifies that the framebuffer should have alpha
//          ES_WINDOW_DEPTH       - specifies that a depth buffer should be created
//          ES_WINDOW_STENCIL     - specifies that a stencil buffer should be created
//          ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created

GLboolean ESUTIL_API esCreateWindow(ESContext *esContext, const char* title, GLint width, GLint height, GLuint flags)
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

	esContext->width = width;
	esContext->height = height;

	if(!WinCreate(esContext, title))
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


//  esMainLoop()
//
//    Start the main loop for the OpenGL ES application

void ESUTIL_API esMainLoop(ESContext *esContext)
{
	WinLoop(esContext);
}



//  esRegisterDrawFunc()

void ESUTIL_API esRegisterDrawFunc(ESContext *esContext, void (ESCALLBACK *drawFunc) (ESContext*))
{
	esContext->drawFunc = drawFunc;
}

//  esRegisterUpdateFunc()

void ESUTIL_API esRegisterUpdateFunc(ESContext *esContext, void (ESCALLBACK *updateFunc) (ESContext*, float))
{
	esContext->updateFunc = updateFunc;
}


//  esRegisterKeyFunc()

void ESUTIL_API esRegisterKeyFunc(ESContext *esContext,
	void (ESCALLBACK *keyFunc) (ESContext*, unsigned char, bool))
{
	esContext->keyFunc = keyFunc;
}



// esLogMessage()
//    Log an error message to the debug output for the platform

void ESUTIL_API esLogMessage(const char *formatStr, ...)
{
	va_list params;
	char buf[2048];

	va_start(params, formatStr);
	vsprintf_s(buf, sizeof(buf), formatStr, params);

	printf("%s", buf);
	OutputDebugString(buf);

	va_end(params);
}

// Main window procedure
LRESULT WINAPI ESWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT  lRet = 1;

	switch(uMsg)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
	{
					 ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

					 if(esContext && esContext->drawFunc)
						 esContext->drawFunc(esContext);

					 ValidateRect(esContext->hWnd, NULL);
	}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
	{
					   ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

					   if(esContext && esContext->keyFunc)
						   esContext->keyFunc(esContext, (unsigned char)wParam, true);
	}
		break;

	case WM_KEYUP:
	{
					 ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

					 if(esContext && esContext->keyFunc)
						 esContext->keyFunc(esContext, (unsigned char)wParam, false);
	}
		break;

	default:
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;
}

//      Create Win32 instance and window
GLboolean WinCreate(ESContext *esContext, const char *title)
{
	WNDCLASS wndclass = { 0 };
	DWORD    wStyle = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);


	wndclass.style = CS_OWNDC;
	wndclass.lpfnWndProc = (WNDPROC)ESWindowProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = "opengles2.0";

	if(!RegisterClass(&wndclass))
		return FALSE;

	wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

	// Adjust the window rectangle so that the client area has
	// the correct number of pixels
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = esContext->width;
	windowRect.bottom = esContext->height;

	AdjustWindowRect(&windowRect, wStyle, FALSE);

	esContext->hWnd = CreateWindow(
		"opengles2.0",
		title,
		wStyle,
		0,
		0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Set the ESContext* to the GWL_USERDATA so that it is available to the 
	// ESWindowProc
	SetWindowLongPtr(esContext->hWnd, GWL_USERDATA, (LONG)(LONG_PTR)esContext);

	if(esContext->hWnd == NULL)
		return GL_FALSE;

	ShowWindow(esContext->hWnd, TRUE);

	return GL_TRUE;
}



//      Start main windows loop
void WinLoop(ESContext *esContext)
{
	MSG msg = { 0 };
	int done = 0;
	DWORD lastTime = GetTickCount();

	while(!done)
	{
		int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
		DWORD curTime = GetTickCount();
		float deltaTime = (float)(curTime - lastTime) / 1000.0f;
		lastTime = curTime;

		if(gotMsg)
		{
			if(msg.message == WM_QUIT)
			{
				done = 1;
			} else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else
			SendMessage(esContext->hWnd, WM_PAINT, 0, 0);

		// Call update function if registered
		if(esContext->updateFunc != NULL)
			esContext->updateFunc(esContext, deltaTime);
	}
}
