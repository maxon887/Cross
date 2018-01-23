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
#pragma once
#include "Cross.h"

namespace cross{

/*	Container class for data loaded from file system.
	To load a File use System class */
class File {
public:
	/* Returns directory from full filename */
	static String PathFromFile(const String& filePath);
	/* Return naked filename from filepath */
	static String FileFromPath(const String& filename);
	/* Returns extension from filename or full file path */
	static String ExtensionFromFile(const String& file);
	/* Returns raw file name without extension (ex "log" for "log.txt") */
	static String FileWithoutExtension(const String& file);

	~File();

	String name	= String();
	Byte* data	= null;
	U32 size	= 0;
};

}
