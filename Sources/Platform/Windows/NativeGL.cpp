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
#ifdef OPENGL
#include "Cross.h"
#include "Native.h"
#include "LauncherWIN.h"
#include "GraphicsGL.h"
#include "Game.h"
#include "Input.h"
#include "Config.h"
#include "Graphics2D.h"
#include "Screen.h"
#include "resource.h"

using namespace cross;

Game* cross::game = NULL;

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

int NativeGL_GO(){
	HWND wnd = WinCreate();
	MSG msg;
	try{
		LauncherWIN launcherWin(wnd);
		launcher = &launcherWin;
		game = CrossMain(launcher);

		int winX = config->LoadInt("WIN_POS_X", 0);
		int winY = config->LoadInt("WIN_POS_Y", 0);
		int winWidth = config->LoadInt("WIN_WIDTH", 500);
		int winHeight = config->LoadInt("WIN_HEIGHT", 500);
		launcherWin.ResizeWindow(winX, winY, winWidth, winHeight);
		input->KeyReleased += MakeDelegate(&launcherWin, &LauncherWIN::KeyReleasedHandle);

		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 16;
		pfd.cDepthBits = 16;

		HDC dc;
		HGLRC renderContext;
		unsigned int pixelFormat;
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

		gfxGL = new GraphicsGL();
		gfx2D = new Graphics2D();
		game->Start();
		game->SetScreen(game->GetStartScreen());

		ZeroMemory(&msg, sizeof(MSG));
		while(msg.message != WM_QUIT) {
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			game->Update();
			SwapBuffers(dc);
		}
		game->GetCurrentScreen()->Stop();
		game->Stop();
		delete game;
		delete gfx2D;
		delete gfxGL;
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