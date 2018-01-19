/*	Copyright © 2018 Maksim Lukyanov

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
#include "Native.h"
#include "Cross.h"
#include "Game.h"
#include "Screen.h"
#include "Input.h"
#include "Config.h"
#include "resource.h"
#include "WINSystem.h"
#include "GLES.h"
#include "Platform/CrossEGL.h"

using namespace cross;

RECT GetLocalCoordinates(HWND hWnd){
	RECT win, client;
	GetWindowRect(hWnd, &win);
	GetClientRect(hWnd, &client);
	POINT diff;
	diff.x = (win.right - win.left) - client.right;
	diff.y = (win.bottom - win.top) - client.bottom;
	win.right -= diff.x;
	win.bottom -= diff.y;
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&win, 2);

	return win;
}
	
LRESULT CALLBACK WinProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam){
	static bool mouseDown = false;
	switch(msg){
	case WM_LBUTTONDOWN:{
		SetCapture(wnd);
		float targetX = (short)LOWORD(lParam);
		float targetY = (short)HIWORD(lParam);
		mouseDown = true;
		input->TargetActionDown.Emit(targetX, targetY, 0);
		break;
	}
	case WM_RBUTTONDOWN:{
		SetCapture(wnd);
		float targetX = (short)LOWORD(lParam);
		float targetY = (short)HIWORD(lParam);
		mouseDown = true;
		input->TargetActionDown.Emit(targetX, targetY, 1);
		break;
	}
	case WM_MBUTTONDOWN:{
		SetCapture(wnd);
		float targetX = (short)LOWORD(lParam);
		float targetY = (short)HIWORD(lParam);
		mouseDown = true;
		input->TargetActionDown.Emit(targetX, targetY, 2);
		break;
	}
	case WM_MOUSEMOVE:{
		float targetX = (short)LOWORD(lParam);
		float targetY = (short)HIWORD(lParam);
		input->MousePosition.x = targetX;
		input->MousePosition.y = targetY;
		if(mouseDown){
			input->TargetActionMove.Emit(targetX, targetY, 0);
		}
		break;
	}
	case WM_LBUTTONUP:{
		ReleaseCapture();
		float targetX = (short)LOWORD(lParam);
		float targetY = (short)HIWORD(lParam);
		mouseDown = false;
		input->TargetActionUp.Emit(targetX, targetY, 0);
		break;
	}
	case WM_RBUTTONUP: {
		ReleaseCapture();
		float targetX = (short)LOWORD(lParam);
		float targetY = (short)HIWORD(lParam);
		mouseDown = false;
		input->TargetActionUp.Emit(targetX, targetY, 1);
		break;
	}
	case WM_MBUTTONUP: {
		ReleaseCapture();
		float targetX = (short)LOWORD(lParam);
		float targetY = (short)HIWORD(lParam);
		mouseDown = false;
		input->TargetActionUp.Emit(targetX, targetY, 2);
		break;
	}
	case WM_MOUSEWHEEL:{
		short delta = (short)HIWORD(wParam); 
		input->MouseWheelRoll.Emit((float)delta);
		break;
	}
	case WM_KEYDOWN:
        input->KeyPressed.Emit((cross::Key)wParam);
		break;
	case WM_SYSKEYDOWN:
		input->KeyPressed.Emit((cross::Key)wParam);
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		input->KeyReleased.Emit((cross::Key)wParam);
		break;
	case WM_CHAR:
		input->CharEnter.Emit(wParam);
		break;
	case WM_MOVE:{
		int x = LOWORD(lParam) - 8;
		int y = HIWORD(lParam) - 30;
		WINSystem* winSys = (WINSystem*)cross::system;
		winSys->SetWindowPosition(x, y);
		break;
	}
	case WM_SIZE:{
		RECT winRect = GetLocalCoordinates(wnd);
		int width = winRect.right - winRect.left;
		int height = winRect.bottom - winRect.top;
		if(width > 0 && height > 0){
			WINSystem* winSys = (WINSystem*)cross::system;
			winSys->SetWindowSize(width, height);
		}
		break;
	}
	case WM_KILLFOCUS:
		if(game) {
			game->Suspend();
		}
		break;
	case WM_SETFOCUS:
		if(game) {
#ifdef GLES
			if(!crossEGL->IsContextCreated()) {
				crossEGL->CreateContext(true);
			} else {
				crossEGL->DestroyContext(false);
				crossEGL->CreateContext(false);
			}
#endif
			game->Resume();
		}
		break;
	case WM_CLOSE: {
		RECT winRect = GetLocalCoordinates(wnd);
		config->SetInt("WIN_POS_X", winRect.left);
		config->SetInt("WIN_POS_Y", winRect.top);
		config->SetInt("WIN_WIDTH", winRect.right - winRect.left);
		config->SetInt("WIN_HEIGHT", winRect.bottom - winRect.top);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(wnd, msg, wParam, lParam);
}

HWND WinCreate(){
	HINSTANCE instance = GetModuleHandle(NULL);
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = (HICON)LoadImageA(instance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0);
	wc.hIcon = (HICON)LoadImageA(instance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 64, 64, 0);
	wc.lpfnWndProc = WinProc;
	wc.hInstance = instance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "Cross++";
	RegisterClassEx(&wc);
	
	HWND hwnd = CreateWindow(wc.lpszClassName, "Cross++", WS_OVERLAPPEDWINDOW, 300, 0, 0, 0, NULL, NULL, instance, NULL);
	return hwnd;
}
