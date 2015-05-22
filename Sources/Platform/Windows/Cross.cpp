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

#include "Cross.h"
#include "Graphics.h"
#include "Input.h"
#include "Screen.h"
#include "resource.h"

static Game* mGame;
static Input* input;

void ClientResize(HWND hWnd, int nX, int nY, int nWidth, int nHeight)
{
	RECT rcClient, rcWind;
	POINT ptDiff;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(hWnd,nX, nY, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
}

RECT GetLocalCoordinates(HWND hWnd){
    RECT Rect;
    GetWindowRect(hWnd, &Rect);
    MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT) &Rect, 2);
    return Rect;
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
	RECT winRect;
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		input->input_state = true;
		input->input_loc.x = (short)LOWORD(lParam);
		input->input_loc.y = (short)HIWORD(lParam);
		break;
	case WM_MOUSEMOVE:
		input->input_loc.x = LOWORD(lParam);
		input->input_loc.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		input->input_state = false;
		break;
	case WM_CLOSE:
		winRect = GetLocalCoordinates(wnd);
		mGame->saver->SaveInt("WIN_POS_X", winRect.left);
		mGame->saver->SaveInt("WIN_POS_Y", winRect.top);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(wnd, msg, wParam, lParam);
}

int CrossMain(Game* game, HINSTANCE instance, int winShow){
	mGame = game;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = (HICON)LoadImageA(instance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);
	wc.hIcon = (HICON)LoadImageA(instance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0);
	wc.lpfnWndProc = WinProc;
	wc.hInstance = instance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "Cross++";
	RegisterClassEx(&wc);

	HWND wnd = CreateWindow(wc.lpszClassName, "Cross++", WS_OVERLAPPEDWINDOW, 300, 0, 0, 0, NULL, NULL, instance, NULL);
	LauncherWIN* launcher = (LauncherWIN*)game->launcher;
	launcher->SetHWND(wnd);
	int winX = game->saver->LoadInt("WIN_POS_X");
	int winY = game->saver->LoadInt("WIN_POS_Y");
	ClientResize(wnd, winX, winY, launcher->GetTargetWidth(), launcher->GetTargetHeight());

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

	Graphics* graphics = new Graphics(game);
	input = game->input;
	game->graphics = graphics;
	game->input = input;
	game->Start();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		game->Update();
		SwapBuffers(dc);
	}
	return msg.wParam;
}