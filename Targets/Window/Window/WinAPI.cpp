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

#include <Windows.h>
#include <time.h>
#include "LauncherWIN.h"
#include "Graphics.h"
#include "SnakyGame.h"
#include "Demo.h"

void ClientResize(HWND hWnd, int nWidth, int nHeight)
{
	RECT rcClient, rcWind;
	POINT ptDiff;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(hWnd,rcWind.left, rcWind.top, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
}

void ShowLastError(){
		DWORD error = GetLastError();
		LPSTR str = 0;
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER |
						FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						error,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                              // Default language
						str,
						0,
						NULL );
		MessageBox(NULL, str, "Error", MB_OK);
}

LRESULT CALLBACK WinProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(wnd, msg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE instancePrev, LPSTR args, int winShow){
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WinProc;
	wc.hInstance = instance;
	wc.hCursor = LoadCursor(instance, IDC_ARROW);
	wc.lpszClassName = "Cross++";
	RegisterClassEx(&wc);

	LauncherWIN* launcher = new LauncherWIN();

	HWND wnd = CreateWindow(wc.lpszClassName, "Cross++", WS_OVERLAPPEDWINDOW, 300, 0, 0, 0, NULL, NULL, instance, NULL);
	ClientResize(wnd, launcher->GetTargetWidth(), launcher->GetTargetHeight());

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	HDC dc;
	HGLRC renderContext;
	GLuint pixelFormat;
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

	ShowWindow(wnd, winShow);

	//SnakyGame* game = new SnakyGame(launcher);.
	Demo* game = new Demo(launcher);
	Graphics* graphics = new Graphics(game);
	game->graphics = graphics;
	Debuger* debuger = new Debuger(game);
	game->Start();

	clock_t render_time = 0;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		render_time = clock() - render_time;
		clock_t rend = render_time;
		render_time = clock();
		game->GetCurrentScreen()->Update(rend / (float)CLOCKS_PER_SEC);
		debuger->Display(rend / (float)CLOCKS_PER_SEC);
		clock_t up = clock() - render_time;
		debuger->SetUpdateTime(up / (float)CLOCKS_PER_SEC);
		SwapBuffers(dc);
	}
	return msg.wParam;
}