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
#include "System.h"
#include "File.h"

#include <stdarg.h>
#include <cstring>

#if defined(WIN) || defined(EDITOR) || defined(MACOS)
#	define DEFAULT_SCREEN_DPI 96.f
#else
#	define DEFAULT_SCREEN_DPI 160.f
#endif

using namespace cross;

void EvokeAlert(const char* filename, unsigned int line, const char* msg, ...) {
	va_list args;
	va_start(args, msg);
	system->Alert(filename, line, msg, args);
}

File* System::LoadFile(const String& filename) {
	FILE* f = fopen(filename.ToCStr(), "rb");
	CROSS_RETURN(f, nullptr, "Can not open file %s", filename.ToCStr());
	File* file = new File();
	file->name = filename;
	fseek(f, 0, SEEK_END);
	file->size = (U32)ftell(f);
	fseek(f, 0, SEEK_SET);
	file->data = new Byte[file->size];
	U64 read = fread(file->data, sizeof(Byte), file->size, f);
	CROSS_ASSERT(file->size == read, "File %s not read properly", file->name.ToCStr());
	fclose(f);
	return file;
}

File* System::LoadAssetFile(const String& filename) {
	return LoadFile(AssetsPath() + filename);
}

File* System::LoadDataFile(const String& filename) {
	return LoadFile(DataPath() + filename);
}

void System::SaveFile(File* file) {
	FILE* f = fopen(file->name.ToCStr(), "wb");
	CROSS_FAIL(f, "Can not open file for writing: %s", file->name.ToCStr());
	U64 written = fwrite(file->data, 1, file->size, f);
	CROSS_ASSERT(file->size == written, "Can not write to file %s", file->name.ToCStr());
	fclose(f);
}

void System::SaveDataFile(File* file) {
	file->name = DataPath() + file->name;
	SaveFile(file);
}

bool System::IsDataFileExists(const String& filename) {
	String fullpath = DataPath() + filename;
	FILE* f = fopen(fullpath.ToCStr(), "r");
	bool result = f != nullptr;
	if(result) {
		fclose(f);
	}
	return result;
}

bool System::Alert(const String& msg) {
	Messagebox("Something goes wrong", msg.ToCStr());
	return false;
}

void System::Alert(const char* filename, unsigned int line, const char* msg, va_list list) {
	auto it = asserts_hashes.find(line);
	if(it == asserts_hashes.end()) {
		char buffer[4096];
		vsprintf(buffer, msg, list);
		Log(buffer);
#ifdef CROSS_DEBUG
		String str = buffer;
		str += "\n";
		str += "File: ";
		str += filename;
		str += "\n";
		str += "Line: ";
		str += line;
		if(Alert(str)) {
			asserts_hashes.insert(line);
		}
#else
		asserts_hashes.insert(line);
		LogIt("\tAssertion Failed");
		String str = msg;
		str += "\n";
		str += "File: ";
		str += filename;
		str += "\n";
		str += "Line: ";
		str += line;
		LogIt(str.ToCStr());
#endif
	}
}

void System::Sleep(float milis) {
	CROSS_ASSERT(false, "Function System::Speep() does not implemented");
}

void System::Messagebox(const String& title, const String& msg) {
    LogIt("System message box not implemented. Can not show message to user");
}

float System::GetScreenDPI() {
	return DEFAULT_SCREEN_DPI;
}

String System::GetClipboard() {
	CROSS_ASSERT(false, "System::GetClipboard() does not implemented");
	return "";
}

void System::SetClipboard(const String& data) {
	CROSS_ASSERT(false, "System::SetClipboard() does not implemented");
}

void System::LogIt(const char* format, ...) {
	va_list params;
	char buffer[4096];
	va_start(params, format);
	vsprintf(buffer, format, params);
	Log(buffer);
#ifdef CROSS_DEBUG
	if(log_buffer.size() > 8192) {
		log_buffer.erase(log_buffer.begin(), log_buffer.begin() + 1024);
	}

	if(log_buffer.size() != 0) {
		log_buffer.pop_back();
	}
	log_buffer.insert(log_buffer.end(), buffer, buffer + strlen(buffer));
	log_buffer.push_back('\n');
	log_buffer.push_back(0);
#endif // CROSS_DEBUG
	va_end(params);
}

void System::LogIt(const String& msg) {
	LogIt(msg.ToCStr());
}

void System::LogIt(const Vector3D& vector) {
	LogIt("X - %0.3f, Y - %0.3f, Z - %0.3f", vector.x, vector.y, vector.z);
}

float System::GetScreenScale() {
	return GetScreenDPI() / DEFAULT_SCREEN_DPI;
}

S32 System::GetWindowWidth() const {
    CROSS_RETURN(window_width != -1, -1, "Window size not specified");
	return window_width;
}

S32 System::GetWindowHeight() const {
    CROSS_RETURN(window_height != -1, -1, "Window size not specified");
	return window_height;
}

System::Orientation System::GetDeviceOrientation() const {
	if(window_width > window_height){
		return Orientation::LANDSCAPE;
	}else{
		return Orientation::PORTRAIT;
	}
}

float System::GetAspectRatio() const {
	return GetWindowWidth() / (float)GetWindowHeight();
}

Array<char>& System::GetLogBuffer() {
	return log_buffer;
}

void System::SetWindowSize(S32 width, S32 height) {
	Orientation prevO = GetDeviceOrientation();
	window_width = width;
	window_height = height;
	WindowResized.Emit(width, height);
	if(prevO != GetDeviceOrientation()){
		OrientationChanged.Emit(GetDeviceOrientation());
	}
}
