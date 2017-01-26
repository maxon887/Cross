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
#include "WINSystem.h"
#include "File.h"

#include <fstream>

#define DATA_PATH "Data/"

using namespace cross;

bool DirectoryExists(const char* szPath){
  DWORD dwAttrib = GetFileAttributesA(szPath);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void IntSleep(int milis){
	Sleep(milis);
}

WINSystem::WINSystem(HWND wnd) : 
	wnd(wnd),
	window_pos_x(0),
	window_pos_y(0)
{
	LogIt("LauncherWIN::LauncherWIN(HWND wnd)");
	if(!DirectoryExists(DATA_PATH)){
		CreateDirectoryA(DATA_PATH, NULL);
	}
	char* releaseAsset = "Assets/";
	char* debugAsset = "../../../Assets/";
	char* debugAssetAlt = "../../Assets/";
	char* editorAsset = "../Games/Demo/Assets/";
	if(DirectoryExists(releaseAsset)){
		assets_path = releaseAsset;
	} else if(DirectoryExists(debugAsset)){
		assets_path = debugAsset;
	} else if(DirectoryExists(debugAssetAlt)){
		assets_path = debugAssetAlt;
	} else if(DirectoryExists(editorAsset)){
		assets_path = editorAsset;
	} else {
		throw CrossException("Can't find Assets folder");
	}
}

WINSystem::~WINSystem(){ }

string WINSystem::AssetsPath(){
	return assets_path;
}

string WINSystem::DataPath(){
	return DATA_PATH;
}

U64 WINSystem::GetTime(){
	LARGE_INTEGER freq;
	LARGE_INTEGER crt;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&crt);
	return (crt.QuadPart * 1000000) / freq.QuadPart;
}

void WINSystem::Log(const char* msg){
	OutputDebugStringA(msg);
	OutputDebugStringA("\n");
}

void WINSystem::Sleep(float milis){
	IntSleep((int)(milis + .5));
}

void WINSystem::ShowMessage(const string& msg){
	if(wnd){
		MessageBoxA(wnd, msg.c_str(), "Unhandled Exception", MB_OK | MB_ICONEXCLAMATION);
	}else{
		LogIt(msg.c_str());
	}
}

void WINSystem::ResizeWindow(int posX, int posY, int width, int height){
	if(wnd){
		window_pos_x = posX;
		window_pos_y = posY;
		SetWindowSize(width, height);
		RECT rcClient, rcWind;
		POINT ptDiff;
		GetClientRect(wnd, &rcClient);
		GetWindowRect(wnd, &rcWind);
		ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
		ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
		MoveWindow(wnd, posX, posY, width + ptDiff.x, height + ptDiff.y, TRUE);
	}else{
		throw CrossException("Zero WND. Can not resize native window");
	}
}

void WINSystem::SetWND(HWND wnd){
	this->wnd = wnd;
}

void WINSystem::SetWindowPosition(int x, int y){
	window_pos_x = x;
	window_pos_y = y;
}

void WINSystem::KeyReleasedHandle(Key key){
	switch(key)	{
	case Key::F1:	//16:9
		ResizeWindow(window_pos_x, window_pos_y, 960, 540);
		break;
	case Key::F2:	//9:16
		ResizeWindow(window_pos_x, window_pos_y, 540, 960);
		break;
	case Key::F3:	//16:10
		ResizeWindow(window_pos_x, window_pos_y, 800, 600);
		break;
	case Key::F4:	//4:3
		ResizeWindow(window_pos_x, window_pos_y, 600, 800);
		break;
	default:
		break;
	}
}