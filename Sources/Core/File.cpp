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
#include "File.h"
#include "System.h"

using namespace cross;

string File::PathFromFile(const string& filePath) {
	const size_t last_slash_idx = filePath.rfind('/');
	CROSS_RETURN(std::string::npos != last_slash_idx, "", "Wrong path format");
	return filePath.substr(0, last_slash_idx);
}

string File::FileFromPath(const string& filename) {
	const size_t last_slash_idx = filename.rfind('/');
	if(std::string::npos != last_slash_idx) {
		return filename.substr(last_slash_idx + 1, filename.size());
	} else {
		return filename;
	}
}

string File::ExtensionFromFile(const string& file) {
	return file.substr(file.find_last_of(".") + 1);
}

string File::FileWithoutExtension(const string& file) {
	size_t lastindex = file.find_last_of(".");
	return file.substr(0, lastindex);
}

File::~File(){
	if(data){
		delete[] data;
	}
}