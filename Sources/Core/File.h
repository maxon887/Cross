#pragma once
#include "Cross.h"
#include "Base/String.h"

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
	/* Returns path staring from Assets folder */
	static String FromAbsoluteToAssetPath(const String& absolutePath);

	~File();

	String name	= String();
	Byte* data	= nullptr;
	S64 size	= 0;
};

}
