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
#ifndef GLES
#include "Cross.h"
#include "Native.h"
#include "WINSystem.h"
#include "GraphicsGL.h"
#include "Game.h"
#include "Input.h"
#include "Config.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Screen.h"
#include "System/Debugger.h"
#include "Audio.h"
#include "resource.h"

using namespace cross;

void ShowLastError(){
	DWORD error = GetLastError();
	LPTSTR str;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                              // Default language
		(LPTSTR)&str,
		0,
		NULL);
	MessageBox(NULL, str, "Error", MB_OK);
}

bool EnterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate) {
	DEVMODE fullscreenSettings;
	bool isChangeSuccessful;
	RECT windowBoundary;

	EnumDisplaySettings(NULL, 0, &fullscreenSettings);
	fullscreenSettings.dmPelsWidth = fullscreenWidth;
	fullscreenSettings.dmPelsHeight = fullscreenHeight;
	fullscreenSettings.dmBitsPerPel = colourBits;
	fullscreenSettings.dmDisplayFrequency = refreshRate;
	fullscreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

	SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
	SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
	isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
	ShowWindow(hwnd, SW_MAXIMIZE);

	return isChangeSuccessful;
}

int OpenGL_Main(){
	HWND wnd = WinCreate();
	MSG msg;
	try{
		WINSystem* winSys = new WINSystem(wnd);
		cross::system = winSys;
		cross::system->LogIt("OpenGL API");
		game = CrossMain();

		int winX = config->GetInt("WIN_POS_X", 0);
		int winY = config->GetInt("WIN_POS_Y", 0);
		int winWidth = config->GetInt("WIN_WIDTH", 500);
		int winHeight = config->GetInt("WIN_HEIGHT", 500);
		winSys->ResizeWindow(winX, winY, winWidth, winHeight);
		input->KeyReleased += MakeDelegate(winSys, &WINSystem::KeyReleasedHandle);

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

		EnterFullscreen(wnd, 1920, 1080, 32, 60);
		glViewport(0, 0, 1920, 1080);

		//ShowWindow(wnd, TRUE);

		audio = new Audio();
		gfxGL = new GraphicsGL();
		gfx2D = new Graphics2D();
		gfx3D = new Graphics3D();
		game->Start();
		game->SetScreen(game->GetStartScreen());

		ZeroMemory(&msg, sizeof(MSG));
		while(msg.message != WM_QUIT) {
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			game->EngineUpdate();
			SwapBuffers(dc);
		}
		game->GetCurrentScreen()->Stop();
		game->Stop();
		Debugger::Release();
		delete gfx3D;
		delete gfx2D;
		delete gfxGL;
		delete game;
		delete audio;
		delete cross::system;
#ifdef CROSS_DEBUG
		unsigned long leaked = MemoryManager::Instance()->Dump();
		if(leaked > 0) {
			char buf[256];
			sprintf(buf, "Memory leak.Total bytes = %d\n", leaked);
			OutputDebugString(buf);
			return -1;
		} else {
			OutputDebugString("No memory leak detected\n");
		}
#endif // CROSS_DEBUG

	} catch(Exception &exc) {
		string msg = string(exc.message) +
			+"\nFile: " + string(exc.filename) +
			+"\nLine: " + to_string(exc.line);
		OutputDebugString(msg.c_str());
		MessageBox(wnd, msg.c_str(), "Unhandled Exception", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	return msg.wParam;
}

#endif