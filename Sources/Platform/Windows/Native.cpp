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
	case WM_MOUSEWHEEL:{
		short delta = (short)HIWORD(wParam); 
		if(delta < 0){
			TRIGGER_EVENT(input->MouseWheelUp);
		}else{
			TRIGGER_EVENT(input->MouseWheelDown);
		}
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
			TRIGGER_EVENT(input->KeyReleased, Key::ESCAPE);
			TRIGGER_EVENT(input->KeyReleased, Key::BACK);
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