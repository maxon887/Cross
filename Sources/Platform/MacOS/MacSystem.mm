#include "MacSystem.h"

#include "File.h"

#import <Cocoa/Cocoa.h>

#include <sys/time.h>
#include <dirent.h>

using namespace cross;

MacSystem::MacSystem(const String& workingDir) {
    working_dir = File::PathFromFile(workingDir);
    if(IsDirectoryExists("Assets/")) {
        assets_path = "Assets/";
    } else if(IsDirectoryExists("../Resources/Assets/")) {
        assets_path = "../Resources/Assets/";
    } else {
        CROSS_ASSERT(false, "Can not find Assets directory");
    }
}

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

bool MacSystem::IsDirectoryExists(const cross::String& filepath) {
    DIR* dir = opendir(working_dir + filepath);
    dirent* dr = nullptr;
    if(dir && (dr = readdir(dir))) {
        if(dr->d_type == DT_DIR) {
            return true;
        } else {
            return false;
        }
    } else {
        CROSS_ASSERT(errno == ENOENT, "IsDirectoryExists() error code - #\nDescription - #", errno, strerror(errno));
        return false;
    }
}

Array<String> MacSystem::GetSubDirectories(const String& filepath) {
	Array<String> files;
	DIR* dir = opendir(filepath);
    if(!dir) {
        CROSS_ASSERT(errno == ENOENT, "GetSubDirectories() error code - #\nDescription - #", errno, strerror(errno));
        return files;
    }
	dirent* dr = nullptr;
	while(dir && (dr = readdir(dir))) {
		String name = dr->d_name;
		if(dr->d_type == DT_DIR && name != "." && name != "..") {
			files.push_back(name);
		}
	}
	closedir(dir);
	return files;
}

Array<String> MacSystem::GetFilesInDirectory(const String& filepath) {
	Array<String> files;
	DIR* dir = opendir(filepath);
    if(!dir) {
        CROSS_ASSERT(errno == ENOENT, "GetSubDirectories() error code - #\nDescription - #", errno, strerror(errno));
        return files;
    }
    
	dirent* dr = nullptr;
	while(dir && (dr = readdir(dir))) {
		if(dr->d_type != DT_DIR) {
			files.push_back(dr->d_name);
		}
	}
	closedir(dir);
	return files;
}

bool MacSystem::Alert(const String& msg) {
	NSAlert *alert = [[NSAlert alloc] init];
	[alert setMessageText:[NSString stringWithCString:"Something goes wrong"
								  encoding:[NSString defaultCStringEncoding]]];
	[alert setInformativeText:[NSString stringWithCString:msg.ToCStr()
									  encoding:[NSString defaultCStringEncoding]]];
	[alert addButtonWithTitle:[NSString stringWithCString:"Ok"
							   encoding:[NSString defaultCStringEncoding]]];
	[alert addButtonWithTitle:[NSString stringWithCString:"Skip"
							   encoding:[NSString defaultCStringEncoding]]];
	NSModalResponse response = [alert runModal];
	switch(response) {
		case NSAlertFirstButtonReturn:
			return false;
		case NSAlertSecondButtonReturn:
			return true;
		default:
			return false;
	}
}

void MacSystem::Messagebox(const String& title, const String& msg) {
	NSAlert *alert = [[NSAlert alloc] init];

	[alert setMessageText:[NSString stringWithCString:title.ToCStr()
								  encoding:[NSString defaultCStringEncoding]]];
	[alert setInformativeText:[NSString stringWithCString:msg.ToCStr()
									  encoding:[NSString defaultCStringEncoding]]];

	[alert runModal];
}

void MacSystem::SetScreenDPI(float newDPI) {
    dpi = newDPI;
}

void MacSystem::SetAssetPath(const String& path) {
	assets_path = path;
}
