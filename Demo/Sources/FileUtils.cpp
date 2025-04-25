#include "FileUtils.h"
#include "System.h"
#include "File.h"

Array<String> FileUtils::GetAllFilesOfType(const String& fileExtension) {
	Array<String> aggregator;
	AssembleAllFilesOfType(fileExtension, os->AssetsPath(), aggregator);
	return aggregator;
}

Array<String> FileUtils::GetAllFilesOfTypeInDirectory(const String& fileExtension, const String& directory) {
	Array<String> result;
	Array<String> allFiles = os->GetFilesInDirectory(directory);
	for(const String& file : allFiles) {
		if(File::ExtensionFromFile(file) == fileExtension) {
			String relativeDir = directory;
			relativeDir.Remove(os->AssetsPath());
			result.Add(relativeDir + file);
		}
	}
	return result;
}

void FileUtils::AssembleAllFilesOfType(const String& fileExtension, const String& directory, Array<String>& aggregator) {
	Array<String> files = GetAllFilesOfTypeInDirectory(fileExtension, directory);
	aggregator.Combine(files);
	Array<String> subDirectories = os->GetSubDirectories(directory);
	for(const String& subDir : subDirectories) {
		AssembleAllFilesOfType(fileExtension, directory + subDir + "/", aggregator);
	}
}