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
#include "File.h"
#include "System.h"

using namespace cross;

String File::PathFromFile(const String& filename) {
	S32 lastSlash = filename.FindLast('/');
	if(lastSlash == -1) {
		lastSlash = filename.FindLast('\\');
	}
	CROSS_RETURN(lastSlash != -1, "", "Wrong path format");
	return filename.SubString(0, lastSlash + 1);
}

String File::FileFromPath(const String& filename) {
	S32 lastSlash = filename.FindLast('/');
	if(lastSlash != -1) {
		return filename.SubString(lastSlash + 1, filename.Length());
	} else {
		return filename;
	}
}

String File::ExtensionFromFile(const String& filename) {
	S32 lastDot = filename.FindLast('.');
	if(lastDot != -1) {
		return filename.SubString(lastDot + 1, filename.Length());
	} else {
		return "";
	}
}

String File::FileWithoutExtension(const String& filename) {
	return filename.SubString(0, filename.FindLast('.'));
}

File::~File() {
	if(data){
		delete[] data;
	}
}