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
#include "Exception.h"

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

LauncherWIN::LauncherWIN(HWND wnd){
	LogIt("LauncherWIN::LauncherWIN(HWND)");
	this->wnd = wnd;
	landscape = false;
	target_width = TARGET_WIDTH;
	target_height = TARGET_HEIGHT;
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
	return assets_path;
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
		string msg = "Cannot open file " + filename;
		throw CrossException(msg);
	}
}

File* LauncherWIN::LoadFile(string filename){
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
	} else{
		throw CrossException("Cannot open file " + filename);
	}
}

void LauncherWIN::LogIt(string msg){
	msg += "\n";
	OutputDebugString(msg.c_str());
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

void LauncherWIN::ShowMessage(string msg){
	MessageBox(wnd, msg.c_str(), "Unhandled Exception", MB_OK | MB_ICONEXCLAMATION);
}

void LauncherWIN::LandscapeMode(bool land){
	landscape = land;
}