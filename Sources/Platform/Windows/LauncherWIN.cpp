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
#include "vld.h"

using namespace cross;

bool DirectoryExists(LPCTSTR szPath){
  DWORD dwAttrib = GetFileAttributes(szPath);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void IntSleep(int milis){
	Sleep(milis);
}

LauncherWIN::LauncherWIN(HWND wnd){
	this->wnd = wnd;
	Audio::Init(this);
	landscape = false;
}

LauncherWIN::~LauncherWIN(){
	Audio::Release();
}

int LauncherWIN::GetTargetWidth(){
	if(landscape){
		return TARGET_HEIGHT;
	}else{
		return TARGET_WIDTH;
	}
}

int LauncherWIN::GetTargetHeight(){
	if(landscape){
		return TARGET_WIDTH;
	}else{
		return TARGET_HEIGHT;
	}
}

string LauncherWIN::AssetsPath(){
	LPCTSTR szPath = "../../Assets/";
	if(DirectoryExists(szPath)){
		return szPath;
	}else{
		return "Assets/";
	}
}

string LauncherWIN::DataPath(){
	LPCTSTR szPath = "Debug/Data/";
	if(DirectoryExists(szPath)){
		return szPath;
	}else{
		return "Data/";
	}
}

void LauncherWIN::LogIt(string msg){
	msg += "\n";
	OutputDebugString(msg.c_str());
}

void LauncherWIN::Sleep(float milis){
	IntSleep((int)(milis + .5));
}

void LauncherWIN::ShowMessage(string msg){
	MessageBox(wnd, msg.c_str(), "Exception", MB_OK);
}

void LauncherWIN::LandscapeMode(bool land){
	landscape = land;
}