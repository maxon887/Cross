#include "MacSystem.h"

#include <sys/time.h>
#include <dirent.h>

using namespace cross;

void MacSystem::Log(const char *msg) {
    printf("%s\n", msg);
}

String MacSystem::AssetsPath() {
	return assets_path;
}

String MacSystem::DataPath() {
    return "Data/";
}

U64 MacSystem::GetTime() {
    struct timeval ptv;
    gettimeofday(&ptv, NULL);
    return (ptv.tv_usec + ptv.tv_sec * 1000000LL);
}

float MacSystem::GetScreenDPI() {
    return dpi;
}

Array<String> MacSystem::GetSubDirectories(const String &filepath) {
	Array<String> files;
	DIR* dir = opendir(filepath);
	dirent* dr = nullptr;
	while((dr = readdir(dir)) != nullptr) {
		String name = dr->d_name;
		if(dr->d_type == DT_DIR && name != "." && name != "..") {
			files.push_back(name);
		}
	}
	closedir(dir);
	return files;
}

Array<String> MacSystem::GetFilesInDirectory(const String &filepath) {
	Array<String> files;
	DIR* dir = opendir(filepath);
	dirent* dr = nullptr;
	while((dr = readdir(dir)) != nullptr) {
		if(dr->d_type != DT_DIR) {
			files.push_back(dr->d_name);
		}
	}
	closedir(dir);
	return files;
}

void MacSystem::SetScreenDPI(float newDPI) {
    dpi = newDPI;
}

void MacSystem::SetAssetPath(const String &path) {
	assets_path = path;
}
