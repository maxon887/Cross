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
#pragma once
#include "Cross.h"

namespace cross{
/*	Container class for data loaded from file system 
	To load File see System class */
class File{
public:
	/* Returns directory from full filename */
	static string PathFromFile(const string& filePath);
	/* Return naked filename from filepath */
	static string FileFromPath(const string& filename);
	/* Returns extencsion from filename or full file path */
	static string ExtensionFromFile(const string& file);
	/* Returns raw file name without extencion (ex "log" for "log.txt") */
	static string FileWithoutExtension(const string& file);

	~File();

	string name	= string();
	Byte* data	= NULL;
	U32 size	= 0;
};

}