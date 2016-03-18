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
#include "Native.h"
#include "Cross.h"
#include "Game.h"
#include "Input.h"
#include "Config.h"
#include "resource.h"
#include "Launcher.h"

using namespace cross;

RECT GetLocalCoordinates(HWND hWnd){
	RECT Rect;
	GetWindowRect(hWnd, &Rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&Rect, 2);
	return Rect;
}

void ClientResize(HWND hWnd, int nX, int nY, int nWidth, int nHeight){
	RECT rcClient, rcWind;
	POINT ptDiff;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(hWnd, nX, nY, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
}
	
LRESULT CALLBACK WinProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam){
	static bool mouseDown = false;
	switch(msg){
	case WM_LBUTTONDOWN:{
		float targetX = (short)LOWORD(lParam);
		float targetY = (short)HIWORD(lParam);
		mouseDown = true;
		targetX = targetX / game->GetScaleFactor();
		targetY = game->GetHeight() - targetY / game->GetScaleFactor();
		TRIGGER_EVENT(input->ActionDown, Vector2D(targetX, targetY));
	}break;
	case WM_MOUSEMOVE:{
		if(mouseDown){
			float targetX = (short)LOWORD(lParam);
			float targetY = (short)HIWORD(lParam);
			targetX = targetX / game->GetScaleFactor();
			targetY = game->GetHeight() - targetY / game->GetScaleFactor();
			TRIGGER_EVENT(input->ActionMove, Vector2D(targetX, targetY));
		}
	}break;
	case WM_LBUTTONUP:{
		float targetX = (short)LOWORD(lParam);
		float targetY = (short)HIWORD(lParam);
		mouseDown = false;
		targetX = targetX / game->GetScaleFactor();
		targetY = game->GetHeight() - targetY / game->GetScaleFactor();
		TRIGGER_EVENT(input->ActionUp, Vector2D(targetX, targetY));
	}break;
	case WM_CLOSE:{
		RECT winRect = GetLocalCoordinates(wnd);
		config->SaveInt("WIN_POS_X", winRect.left);
		config->SaveInt("WIN_POS_Y", winRect.top);
	}break;
	case WM_MOUSEWHEEL:{
		short delta = (short)HIWORD(wParam); 
		if(delta < 0){
			TRIGGER_EVENT(input->MouseWheelUp);
		}else{
			TRIGGER_EVENT(input->MouseWheelDown);
		}
	}break;
	case WM_KEYDOWN:
		TRIGGER_EVENT(input->KeyPressed, (cross::Key)wParam);
		break;
	case WM_KEYUP:
		TRIGGER_EVENT(input->KeyReleased, (cross::Key)wParam);
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

HWND WinCreate(){
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

	return CreateWindow(wc.lpszClassName, "Cross++", WS_OVERLAPPEDWINDOW, 300, 0, 0, 0, NULL, NULL, instance, NULL);
}