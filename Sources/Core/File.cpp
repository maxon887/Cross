#include "File.h"
#include "System.h"

using namespace cross;

String File::PathFromFile(const String& filename) {
	S32 lastSlash = filename.FindLast('/');
	if(lastSlash == -1) {
		lastSlash = filename.FindLast('\\');
	}
	if(lastSlash == -1) {
		return "";
	}
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

String File::FromAbsoluteToAssetPath(const String &absolutePath) {
	const String assetFolder = "Assets/";
	String result = absolutePath;
	S32 foundAssetFolder = absolutePath.Find(assetFolder);
	CROSS_RETURN(foundAssetFolder > 0, String(), "This path does not contain Assets folder");
	result.Cut(foundAssetFolder, result.Length());
	result.Remove(assetFolder);
	return result;
}

File::~File() {
	delete[] data;
}