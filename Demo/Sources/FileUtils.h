#pragma once
#include "Cross.h"

using namespace cross;

class FileUtils {
public:
	static Array<String> GetAllFilesOfType(const String& fileExtention);
	static Array<String> GetAllFilesOfTypeInDirectory(const String& fileExtention, const String& directory);

private:
	static void AssembleAllFilesOfType(const String& fileExtention, const String& directory, Array<String>& aggregator);
};
