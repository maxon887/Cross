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
#include "LauncherWIN.h"
#include "resource.h"

#pragma comment(lib, "opengl32.lib")

using namespace cross;

Game* cross::game = NULL;

void ClientResize(HWND hWnd, int nX, int nY, int nWidth, int nHeight){
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
		LPTSTR str;
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						error,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                              // Default language
						(LPTSTR)&str,
						0,
						NULL );
		MessageBox(NULL, str, "Error", MB_OK);
}

LRESULT CALLBACK WinProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam){
	static bool mouseDown = false;
	RECT winRect;
	short targetX;
	short targetY;
	float x;
	float y;
	switch(msg){
	case WM_LBUTTONDOWN:
		targetX = (short)LOWORD(lParam);
		targetY = (short)HIWORD(lParam);
		x = targetX / game->GetScaleFactor();
		y = targetY / game->GetScaleFactor();
		mouseDown = true;
		TRIGGER_EVENT(input->ActionDown, Point(x, y));
		break;
	case WM_MOUSEMOVE:
		if(mouseDown){
			targetX = (short)LOWORD(lParam);
			targetY = (short)HIWORD(lParam);
			x = targetX / game->GetScaleFactor();
			y = targetY / game->GetScaleFactor();
			TRIGGER_EVENT(input->ActionMove, Point(x, y));
		}
		break;
	case WM_LBUTTONUP:
		targetX = (short)LOWORD(lParam);
		targetY = (short)HIWORD(lParam);
		x = targetX / game->GetScaleFactor();
		y = targetY / game->GetScaleFactor();
		mouseDown = false;
		TRIGGER_EVENT(input->ActionUp, Point(x, y));
		break;
	case WM_CLOSE:
		winRect = GetLocalCoordinates(wnd);
		config->SaveInt("WIN_POS_X", winRect.left);
		config->SaveInt("WIN_POS_Y", winRect.top);
		break;
	case WM_KEYDOWN:
		switch(wParam){
		case VK_ESCAPE:
			TRIGGER_EVENT(input->KeyPressed, Key::ESCAPE);
			break;
		case VK_UP:
			TRIGGER_EVENT(input->KeyPressed, Key::UP);
			break;
		case VK_DOWN:
			TRIGGER_EVENT(input->KeyPressed, Key::DOWN);
			break;
		case VK_LEFT:
			TRIGGER_EVENT(input->KeyPressed, Key::LEFT);
			break;
		case VK_RIGHT:
			TRIGGER_EVENT(input->KeyPressed, Key::RIGHT);
			break;
		case VK_SPACE:
			TRIGGER_EVENT(input->KeyPressed, Key::SPACE);
			break;
		case VK_SHIFT:
			TRIGGER_EVENT(input->KeyPressed, Key::SHIFT);
			break;
		}
		break;
	case WM_KEYUP:
		switch(wParam){
		case VK_ESCAPE:
			TRIGGER_EVENT(input->KeyReleased, Key::PAUSE);
			break;
		case VK_UP:
			TRIGGER_EVENT(input->KeyReleased, Key::UP);
			break;
		case VK_DOWN:
			TRIGGER_EVENT(input->KeyReleased, Key::DOWN);
			break;
		case VK_LEFT:
			TRIGGER_EVENT(input->KeyReleased, Key::LEFT);
			break;
		case VK_RIGHT:
			TRIGGER_EVENT(input->KeyReleased, Key::RIGHT);
			break;
		case VK_SPACE:
			TRIGGER_EVENT(input->KeyReleased, Key::SPACE);
			break;
		case VK_SHIFT:
			TRIGGER_EVENT(input->KeyReleased, Key::SHIFT);
			break;
		}
		break;
	case WM_KILLFOCUS:
		game->Suspend();
		break;
	case WM_SETFOCUS:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(wnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE instancePrev, LPSTR args, int winShow){
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
	launcher = new LauncherWIN(wnd);
	game = CrossMain(launcher);
	int winX = config->LoadInt("WIN_POS_X", 0);
	int winY = config->LoadInt("WIN_POS_Y", 0);
	ClientResize(wnd, winX, winY, launcher->GetTargetWidth(), launcher->GetTargetHeight());

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
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

	ShowWindow(wnd, winShow);
	try{
#ifdef C3D
	gfx3D = new Graphics3D();
#else
	graphics = new Graphics(game);
#endif
	}catch(string& msg){
		msg = "Exception: " + msg;
		launcher->LogIt(msg);
		//launcher->ShowMessage(msg);
		return -1;
	}
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
#ifdef C3D
	delete gfx3D;
#endif
	delete graphics;
	delete game;
	delete launcher;
	return msg.wParam;
}