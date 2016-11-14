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
#include "Launcher.h"
#include "File.h"

#include <stdarg.h>
#include <fstream>

using namespace cross;

File* Launcher::LoadFile(const string& filename){
	File* file = new File();
	file->name = filename;
	string filePath = AssetsPath() + filename;
	ifstream fileStream(filePath, istream::binary);
	if(fileStream.is_open()){
		fileStream.seekg(0, fileStream.end);
		file->size = (size_t)fileStream.tellg();
		fileStream.seekg(0, fileStream.beg);
		file->data = new Byte[file->size];
		memset(file->data, 0, file->size);
		fileStream.read((char*)file->data, file->size);
		fileStream.close();
		return file;
	} else {
		throw CrossException("Cannot open file %s", file->name.c_str());
	}
}

void Launcher::SaveFile(File* file){
	string filePath = DataPath() + file->name;
	ofstream fileStream(filePath, istream::binary);
	if(fileStream.is_open()){
		fileStream.write((char*)file->data, file->size);
		fileStream.close();
	}else{
		throw CrossException("Can't open file stream: %s", filePath.c_str());
	}
}

void Launcher::LogIt(const char* format, ...){
	va_list params;
	char buffer[1024];
	va_start(params, format);
	vsprintf(buffer, format, params);
	Log(buffer);
	va_end(params);
}

float Launcher::GetAspectRatio(){
	return GetTargetWidth() / (float)GetTargetHeight();
}

string Launcher::PathFromFile(const string& filePath){
	const size_t last_slash_idx = filePath.rfind('/');
	if(std::string::npos != last_slash_idx){
		return filePath.substr(0, last_slash_idx);
	}else{
		throw CrossException("Wrong path format");
	}
}

string Launcher::ExtensionFromFile(const string& file){
	return file.substr(file.find_last_of(".") +1);
}

string Launcher::FileWithoutExtension(const string& file){
	size_t lastindex = file.find_last_of("."); 
	return file.substr(0, lastindex); 
}