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

#include <fstream>

using namespace cross;

#define DATA_PATH "Data/"

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
	target_width = TARGET_WIDTH;
	target_height = TARGET_HEIGHT;
	if(!DirectoryExists(DATA_PATH)){
		CreateDirectory(DATA_PATH, NULL);
	}
}

LauncherWIN::~LauncherWIN(){
	Audio::Release();
}

void LauncherWIN::SetTargetSize(int width, int height){
	target_width = width;
	target_height = height;
}

int LauncherWIN::GetTargetWidth(){
	if(landscape){
		return target_height;
	}else{
		return target_width;
	}
}

int LauncherWIN::GetTargetHeight(){
	if(landscape){
		return target_width;
	}else{
		return target_height;
	}
}

string LauncherWIN::AssetsPath(){
	LPCTSTR szPath = "../../../Assets/";
	if(DirectoryExists(szPath)){
		return szPath;
	}else{
		return "Assets/";
	}
}

string LauncherWIN::DataPath(){
	return DATA_PATH;
}

unsigned char* LauncherWIN::LoadFile(string filename, int *size){
	string filePath = AssetsPath() + "//" + filename;
	ifstream fileStream(filePath, istream::binary);
	if(fileStream.is_open()){
		fileStream.seekg(0, fileStream.end);
		*size = (size_t)fileStream.tellg();
		fileStream.seekg(0, fileStream.beg);
		char* data = new char[*size];
		ZeroMemory(data, *size);
		fileStream.read((char*)data, *size);
		return (unsigned char*)data;
	}else{
		throw string("Cannot open file " + filename);
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