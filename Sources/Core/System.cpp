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
#include "System.h"
#include "File.h"

#include <stdarg.h>

using namespace cross;

void EvokeAlert(const char* filename, unsigned int line, const char* msg, ...) {
	va_list args;
	va_start(args, msg);
    system->Alert(filename, line, msg, args);
}

File* System::LoadFile(const string& filename){
	FILE* f = fopen(filename.c_str(), "rb");
	CROSS_RETURN(f, NULL, "Can not open file %s", filename.c_str());
	File* file = new File();
	file->name = filename;
	fseek(f, 0, SEEK_END);
	file->size = ftell(f);
	fseek(f, 0, SEEK_SET);
	file->data = new Byte[file->size];
	U32 read = fread(file->data, sizeof(Byte), file->size, f);
	CROSS_ASSERT(file->size == read, "File %s not read properly", file->name.c_str());
	fclose(f);
	return file;
}

File* System::LoadAssetFile(const string& filename){
	return LoadFile(AssetsPath() + filename);
}

File* System::LoadDataFile(const string& filename){
	return LoadFile(DataPath() + filename);
}

void System::SaveFile(File* file){
	FILE* f = fopen(file->name.c_str(), "w");
	CROSS_FAIL(f, "Can not open file for writing: %s", file->name.c_str());
	U32 written = fwrite(file->data, 1, file->size, f);
	CROSS_ASSERT(file->size == written, "Can not write to file %s", file->name.c_str());
	fclose(f);
}

void System::SaveDataFile(File* file){
	file->name = DataPath() + file->name;
	SaveFile(file);
}

bool System::Alert(const string& msg) {
	Messagebox("Something goes wrong", msg.c_str());
	return false;
}

void System::Alert(const char* filename, unsigned int line, const char* msg, va_list list) {
	auto it = asserts_hashes.find(line);
	if(it == asserts_hashes.end()) {
		char buffer[4096];
		vsprintf(buffer, msg, list);
		Log(buffer);
#ifdef CROSS_DEBUG
		string str = buffer;
		str += "\n";
		str += "File: ";
		str += filename;
		str += "\n";
		str += "Line: " + to_string(line);
		if(Alert(str)) {
			asserts_hashes.insert(line);
		}
#else
		asserts_hashes.insert(line);
		LogIt("\tAssertion Failed");
		string str = msg;
		str += "\n";
		str += "File: ";
		str += filename;
		str += "\n";
		str += "Line: " + to_string(line);
		LogIt(str.c_str());
#endif
	}
}

void System::Messagebox(const string& title, const string& msg) {
	LogIt("\t" + title);
	LogIt(msg);
}

bool System::IsMobile() {
	return false;
}

void System::LogIt(const char* format, ...){
	va_list params;
	char buffer[4096];
	va_start(params, format);
	vsprintf(buffer, format, params);
	Log(buffer);
	va_end(params);
}

void System::LogIt(const string& msg){
	LogIt(msg.c_str());
}

void System::LogIt(const Vector3D& vector){
	LogIt("X - %0.3f, Y - %0.3f, Z - %0.3f", vector.x, vector.y, vector.z);
}

S32 System::GetWindowWidth() const{
	return window_width;
}

S32 System::GetWindowHeight() const{
	return window_height;
}

System::Orientation System::GetDeviceOrientation() const{
    if(window_width > window_height){
        return Orientation::LANDSCAPE;
    }else{
        return Orientation::PORTRAIT;
    }
}

float System::GetAspectRatio() const{
	return GetWindowWidth() / (float)GetWindowHeight();
}

void System::SetWindowSize(S32 width, S32 height){
	Orientation prevO = GetDeviceOrientation();
	window_width = width;
	window_height = height;
	WindowResized(width, height);
	if(prevO != GetDeviceOrientation()){
		OrientationChanged(GetDeviceOrientation());
	}
}
