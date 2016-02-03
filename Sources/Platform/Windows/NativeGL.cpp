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

void ClientResize(HWND hWnd, int nX, int nY, int nWidth, int nHeight){
	RECT rcClient, rcWind;
	POINT ptDiff;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(hWnd, nX, nY, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
}

RECT GetLocalCoordinates(HWND hWnd){
	RECT Rect;
	GetWindowRect(hWnd, &Rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&Rect, 2);
	return Rect;
}

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

LRESULT CALLBACK WinProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam){
	static bool mouseDown = false;
	switch(msg){
	case WM_LBUTTONDOWN:{
							short targetX = (short)LOWORD(lParam);
							short targetY = (short)HIWORD(lParam);
							mouseDown = true;
							TRIGGER_EVENT(input->ActionDown, Vector2D(targetX, game->GetHeight() - targetY));
	}break;
	case WM_MOUSEMOVE:{
						  if(mouseDown){
							  short targetX = (short)LOWORD(lParam);
							  short targetY = (short)HIWORD(lParam);
							  TRIGGER_EVENT(input->ActionMove, Vector2D(targetX, game->GetHeight() - targetY));
						  }
	}break;
	case WM_LBUTTONUP:{
						  short targetX = (short)LOWORD(lParam);
						  short targetY = (short)HIWORD(lParam);
						  mouseDown = false;
						  TRIGGER_EVENT(input->ActionUp, Vector2D(targetX, game->GetHeight() - targetY));
	}break;
	case WM_CLOSE:{
					  RECT winRect = GetLocalCoordinates(wnd);
					  config->SaveInt("WIN_POS_X", winRect.left);
					  config->SaveInt("WIN_POS_Y", winRect.top);
	}break;
	case WM_KEYDOWN:
		switch(wParam){
		case VK_ESCAPE:
			TRIGGER_EVENT(input->KeyPressed, Key::BACK);
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
		case VK_CONTROL:
			TRIGGER_EVENT(input->KeyPressed, Key::CONTROL);
			break;
			break;
		}
		break;
	case WM_CHAR:
		switch(wParam){
		case 'w': case 'W':
			TRIGGER_EVENT(input->KeyPressed, Key::W);
			break;
		case 'a': case 'A':
			TRIGGER_EVENT(input->KeyPressed, Key::A);
			break;
		case 's': case 'S':
			TRIGGER_EVENT(input->KeyPressed, Key::S);
			break;
		case 'd': case 'D':
			TRIGGER_EVENT(input->KeyPressed, Key::D);
			break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		switch(wParam){
		case VK_ESCAPE:
			TRIGGER_EVENT(input->KeyReleased, Key::PAUSE);
			TRIGGER_EVENT(input->KeyReleased, Key::ESCAPE);
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
		case VK_CONTROL:
			TRIGGER_EVENT(input->KeyReleased, Key::CONTROL);
			break;
		case 'w': case 'W':
			TRIGGER_EVENT(input->KeyReleased, Key::W);
			break;
		case 'a': case 'A':
			TRIGGER_EVENT(input->KeyReleased, Key::A);
			break;
		case 's': case 'S':
			TRIGGER_EVENT(input->KeyReleased, Key::S);
			break;
		case 'd': case 'D':
			TRIGGER_EVENT(input->KeyReleased, Key::D);
			break;
		default:
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

int OPENGL_GO(){
	HINSTANCE instance = GetModuleHandle(NULL);
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
	MSG msg;
	try{
		LauncherWIN launcherWin(wnd);
		launcher = &launcherWin;
		game = CrossMain(launcher);

		int winX = config->LoadInt("WIN_POS_X", 0);
		int winY = config->LoadInt("WIN_POS_Y", 0);
		ClientResize(wnd, winX, winY, launcher->GetTargetWidth(), launcher->GetTargetHeight());

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
		game->Init();
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
		MessageBox(wnd, msg.c_str(), "Unhandled Exception", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	return msg.wParam;
}

#endif