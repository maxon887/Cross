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
#include "LauncherWIN.h"
#include "File.h"
#include "Native.h"

#include <fstream>

#define DATA_PATH "Data/"

using namespace cross;

bool DirectoryExists(LPCTSTR szPath){
  DWORD dwAttrib = GetFileAttributes(szPath);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void IntSleep(int milis){
	Sleep(milis);
}

LauncherWIN::LauncherWIN(HWND wnd) : 
	wnd(wnd),
	window_pos_x(0),
	window_pos_y(0),
	window_width(0),
	window_height(0)
{
	LogIt("LauncherWIN::LauncherWIN(HWND wnd)");
	if(!DirectoryExists(DATA_PATH)){
		CreateDirectory(DATA_PATH, NULL);
	}
	char* releaseAsset = "Assets/";
	char* debugAsset = "../../../Assets/";
	char* debugAssetAlt = "../../Assets/";
	if(DirectoryExists(releaseAsset)){
		assets_path = releaseAsset;
	} else if(DirectoryExists(debugAsset)){
		assets_path = debugAsset;
	} else if(DirectoryExists(debugAssetAlt)){
		assets_path = debugAssetAlt;
	} else {
		throw CrossException("Can't find Assets folder");
	}
}

LauncherWIN::~LauncherWIN(){ }

int LauncherWIN::GetTargetWidth(){
	return window_width;
}

int LauncherWIN::GetTargetHeight(){
	return window_height;
}

string LauncherWIN::AssetsPath(){
	return assets_path;
}

string LauncherWIN::DataPath(){
	return DATA_PATH;
}

File* LauncherWIN::LoadFile(const string& filename){
	File* file = new File();
	file->name = filename;
	string filePath = AssetsPath() + "//" + filename;
	ifstream fileStream(filePath, istream::binary);
	if(fileStream.is_open()){
		fileStream.seekg(0, fileStream.end);
		file->size = (size_t)fileStream.tellg();
		fileStream.seekg(0, fileStream.beg);
		file->data = new byte[file->size];
		ZeroMemory(file->data, file->size);
		fileStream.read((char*)file->data, file->size);
		return file;
	} else {
		throw CrossException("Cannot open file %s", file->name.c_str());
	}
}

void LauncherWIN::LogIt(const string& msg){
	OutputDebugString(msg.c_str());
	OutputDebugString("\n");
}

void LauncherWIN::LogIt(const char* formatStr, ...){
	va_list params;
	char buffer[1024];
	va_start(params, formatStr);
	vsprintf_s(buffer, sizeof(buffer), formatStr, params);
	OutputDebugString(buffer);
	OutputDebugString("\n");
	va_end(params);
}

void LauncherWIN::Sleep(float milis){
	IntSleep((int)(milis + .5));
}

void LauncherWIN::ShowMessage(const string& msg){
	MessageBox(wnd, msg.c_str(), "Unhandled Exception", MB_OK | MB_ICONEXCLAMATION);
}

void LauncherWIN::ResizeWindow(int posX, int posY, int width, int height){
	window_pos_x = posX;
	window_pos_y = posY;
	window_width = width;
	window_height = height;
	RECT rcClient, rcWind;
	POINT ptDiff;
	GetClientRect(wnd, &rcClient);
	GetWindowRect(wnd, &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(wnd, posX, posY, width + ptDiff.x, height + ptDiff.y, TRUE);
}

void LauncherWIN::SetWindowPosition(int x, int y){
	window_pos_x = x;
	window_pos_y = y;
}

void LauncherWIN::SetWindowSize(int width, int height){
	window_width = width;
	window_height = height;
}

void LauncherWIN::KeyReleasedHandle(Key key){
	switch(key)	{
	case Key::F12:
		ResizeWindow(window_pos_x, window_pos_y, window_height, window_width);
		break;
	case Key::F1:	//16:9
		ResizeWindow(window_pos_x, window_pos_y, 960, 540);
		break;
	case Key::F2:	//5:3
		ResizeWindow(window_pos_x, window_pos_y, 1000, 600);
		break;
	case Key::F3:	//16:10
		ResizeWindow(window_pos_x, window_pos_y, 960, 600);
		break;
	case Key::F4:	//4:3
		ResizeWindow(window_pos_x, window_pos_y, 800, 600);
		break;
	default:
		break;
	}
}