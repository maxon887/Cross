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
#include <fstream>

#define C_IMP

using namespace cross;

File* System::LoadFile(const string& filename){
	File* file = new File();
	file->name = filename;
	//C realization
#ifdef C_IMP
	FILE* f = fopen(filename.c_str(), "rb");
	if(f){
		fseek(f, 0, SEEK_END);
		file->size = ftell(f);
		fseek(f, 0, SEEK_SET);
		file->data = new Byte[file->size];
		if(file->size == fread(file->data, sizeof(Byte), file->size, f)){
			fclose(f);
			return file;
		}else{
			throw CrossException("Can not read file %s", file->name.c_str());
		}
	}else{
		throw CrossException("Can not open file %s", file->name.c_str());
	}
#endif
	//C++ implementation
#ifdef CPP_IMP
	file->name = filename;
	ifstream fileStream(filename, istream::binary);
	if(fileStream.is_open()) {
		fileStream.seekg(0, fileStream.end);
		file->size = (size_t)fileStream.tellg();
		fileStream.seekg(0, fileStream.beg);
		file->data = new Byte[file->size];
		memset(file->data, 0, file->size);
		fileStream.read((char*)file->data, file->size);
		fileStream.close();
		return file;
	}else{
		throw CrossException("Can not open file %s", file->name.c_str());
	}
#endif
}

File* System::LoadAssetFile(const string& filename){
	return LoadFile(AssetsPath() + filename);
}

File* System::LoadDataFile(const string &filename){
	return LoadFile(DataPath() + filename);
}

void System::SaveFile(File* file){
#ifdef C_IMP
	FILE* f = fopen(file->name.c_str(), "w");
	if(f) {
		if(file->size == fwrite(file->data, 1, file->size, f)){
			fclose(f);
		}else{
			throw CrossException("Can not write to file %s", file->name.c_str());
		}
	}else{
		throw CrossException("Can not open file for writing: %s", file->name.c_str());
	}
#endif // C_IMP

#ifdef CPP_IMP
	string filePath = DataPath() + file->name;
	ofstream fileStream(filePath, istream::binary);
	if(fileStream.is_open()) {
		fileStream.write((char*)file->data, file->size);
		fileStream.close();
	} else {
		throw CrossException("Can not open file stream: %s", filePath.c_str());
	}
#endif // CPP_IMP
}

void System::SaveDataFile(File* file){
	file->name = DataPath() + file->name;
	SaveFile(file);
}

void System::Messagebox(const string& title, const string& msg) {
	LogIt("\t" + title);
	LogIt(msg);
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

void System::Assert(bool condition, const char* msg, const char* file, int line) {
#ifdef CROSS_DEBUG
	if(condition == false){
		string str = msg;
		str += "\n";
		str += "File: ";
		str += file;
		str += "\n";
		str += "Line: " + to_string(line);
		Messagebox("Assertion Failed", str.c_str());
	}
#else
	auto it = asserts_hashes.find(line);
	if(it == asserts_hashes.end()) {
		asserts_hashes.insert(line);
		LogIt("\tAssertion Failed");
		string str = msg;
		str += "\n";
		str += "File: ";
		str += file;
		str += "\n";
		str += "Line: " + to_string(line);
		LogIt(str.c_str());
	}
#endif
}
