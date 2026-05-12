#ifndef GLES
#include "Cross.h"
#include "Native.h"
#include "WINSystem.h"
#include "Internals/GraphicsGL.h"
#include "Internals/Audio.h"
#include "Game.h"
#include "Input.h"
#include "Config.h"
#include "Screen.h"
#include "Utils/Debugger.h"
#include "Internals/MemoryManager.h"
#include "resource.h"

using namespace cross;

void ShowLastError() {
	DWORD error = GetLastError();
	LPTSTR str;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),							  // Default language
		(LPTSTR)&str,
		0,
		nullptr);
	MessageBox(nullptr, str, "Error", MB_OK);
}

int OpenGL_Main() {
#ifdef CROSS_MEMORY_PROFILE
	MemoryManager::dead = false;
#endif // CROSS_MEMORY_PROFILE

	HWND wnd = WinCreate();
	MSG msg;

	WINSystem* winSys = CREATE WINSystem(wnd);
	os = winSys;
	game = CrossMain();

	int winX = config->GetInt("WIN_POS_X", 100);
	int winY = config->GetInt("WIN_POS_Y", 100);
	int winWidth = config->GetInt("WIN_WIDTH", 960);
	int winHeight = config->GetInt("WIN_HEIGHT", 512);
	winSys->ResizeWindow(winX, winY, winWidth, winHeight);
	input->KeyReleased.Connect(winSys, &WINSystem::KeyReleasedHandle);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	HDC dc;
	HGLRC renderContext;
	S32 pixelFormat;
	DWORD error;

	dc = GetDC(wnd);
	if(!dc)
		ShowLastError();

	pixelFormat = ChoosePixelFormat(dc, &pfd);
	if(!pixelFormat)
		ShowLastError();

	error = SetPixelFormat(dc, pixelFormat, &pfd);
	if(!error)
		ShowLastError();

	renderContext = wglCreateContext(dc);
	if(!renderContext)
		ShowLastError();

	error = wglMakeCurrent(dc, renderContext);
	if(!error)
		ShowLastError();

	ShowWindow(wnd, TRUE);

	audio = CREATE Audio();
	gfxGL = CREATE GraphicsGL();
	game->Start();

	ZeroMemory(&msg, sizeof(MSG));
	while(msg.message != WM_QUIT) {
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(msg.message == WM_QUIT) break;
		}
		game->EngineUpdate();
		SwapBuffers(dc);
	}
	game->GetCurrentScreen()->Stop();
	game->Stop();

	input->KeyReleased.Disconnect(winSys, &WINSystem::KeyReleasedHandle);

	delete gfxGL;
	delete game;
	delete audio;
	delete os;
	Debugger::Release();
#ifdef CROSS_MEMORY_PROFILE
	MemoryManager::Instance()->Dump();
	MemoryManager::dead = true;
#endif // CROSS_MEMORY_PROFILE
	return (int)msg.wParam;
}

#endif