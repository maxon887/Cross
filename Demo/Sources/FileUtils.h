#pragma once
#include "Cross.h"
#include "Base/Array.h"
#include "Base/String.h"

using namespace cross;

class FileUtils {
public:
	static Array<String> GetAllFilesOfType(const String& fileExtension);
	static Array<String> GetAllFilesOfTypeInDirectory(const String& fileExtension, const String& directory);

private:
	static void AssembleAllFilesOfType(const String& fileExtension, const String& directory, Array<String>& aggregator);
};
