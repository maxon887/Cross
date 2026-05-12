#include "MacSystem.h"

#include "File.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#import <Cocoa/Cocoa.h>
#pragma clang diagnostic pop
#include <sys/time.h>
#include <sys/stat.h>
#include <filesystem>

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

using namespace cross;

MacSystem::MacSystem(const String& executable) {
    LogIt("MacSystem::MacSystem()");
    if(System::IsDirectoryExists("Assets/")) {
        assets_path = "Assets/";
    } else if(System::IsDirectoryExists("../../../Assets/")) {
        assets_path = "../../../Assets/";
    } else if(System::IsDirectoryExists("../../../../Assets/")) {
        assets_path = "../../../../Assets/";
    } else if(System::IsDirectoryExists("../../../../../Assets/")) {
        assets_path = "../../../../../Assets/";
    } else {
		String workingDir = File::PathFromFile(executable);
		if(System::IsDirectoryExists(workingDir + "../Resources/Assets/")) {
			assets_path = workingDir + "../Resources/Assets/";
		} else {
			CROSS_ASSERT(false, "Can not find Assets directory");
		}
    }
	
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	NSString *applicationSupportDirectory = [paths firstObject];
	data_path = [applicationSupportDirectory cStringUsingEncoding:[NSString defaultCStringEncoding]];
	data_path += "/Data/";
	LogIt("\tdata_path - '#'", data_path);
}

String MacSystem::AssetsPath() {
	return assets_path;
}

String MacSystem::DataPath() {
	return data_path;
}

U64 MacSystem::GetTime() {
    struct timeval ptv{};
    gettimeofday(&ptv, NULL);
    return (ptv.tv_usec + ptv.tv_sec * 1000000LL);
}

float MacSystem::GetScreenDPI() {
    return dpi;
}

bool MacSystem::Alert(const String& msg) {
	NSAlert *alert = [[NSAlert alloc] init];
	alert.accessoryView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 400, 0)];
	[alert setMessageText:[NSString stringWithCString:"Something goes wrong"
								  encoding:[NSString defaultCStringEncoding]]];
	[alert setInformativeText:[NSString stringWithCString:msg.ToCStr()
									  encoding:[NSString defaultCStringEncoding]]];
	[alert addButtonWithTitle:[NSString stringWithCString:"Continue"
							   encoding:[NSString defaultCStringEncoding]]];
	[alert addButtonWithTitle: [NSString stringWithCString:"Abort"
							   encoding:[NSString defaultCStringEncoding]]];
	[alert addButtonWithTitle:[NSString stringWithCString:"Skip"
							   encoding:[NSString defaultCStringEncoding]]];
	NSModalResponse response = [alert runModal];
	switch(response) {
		case NSAlertFirstButtonReturn:
			return false;
		case NSAlertSecondButtonReturn:
			*((volatile unsigned int*)0) = 0xDEAD;
		case NSAlertThirdButtonReturn:
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

void MacSystem::Sleep(float milis) {
	usleep((useconds_t)(milis * 1000.f));
}

String MacSystem::GetClipboard() {
	NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
	NSString* clipboard = [pasteboard stringForType:NSPasteboardTypeString];
	return [clipboard cStringUsingEncoding:[NSString defaultCStringEncoding]];
}

void MacSystem::SetClipboard(const String& data) {
	NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
	[pasteboard declareTypes:[NSArray arrayWithObject:NSStringPboardType] owner:nil];
	NSString* nsString = [NSString stringWithCString:data];
	[pasteboard setString:nsString forType:NSStringPboardType];
}

String MacSystem::OpenFileDialog(const String& extension, bool saveDialog) {
	NSURL* fileURL = [[NSURL alloc] init];
	if(!saveDialog) {
		NSOpenPanel* openDialog = [NSOpenPanel openPanel];
		[openDialog setCanChooseFiles:YES];
		[openDialog setAllowsMultipleSelection:NO];
		[openDialog setCanChooseDirectories:NO];
		
		if(extension != "*.*") {
			String shortExtension = extension.SubString(2, 5);
			NSString* nsExtension = [NSString stringWithCString:shortExtension];
			NSArray* fileTypes = [NSArray arrayWithObjects:nsExtension,nil];
			[openDialog setAllowedFileTypes:fileTypes];
		}
		if([openDialog runModal] == NSOKButton) {
			NSArray* urls = [openDialog URLs];
			if([urls count] > 0) {
				fileURL = [urls objectAtIndex:0];
				if(fileURL.absoluteString.length > 0) {
					String result = [fileURL fileSystemRepresentation];
					result = File::FromAbsoluteToAssetPath(result);
					return result;
				}
			}
		}
	} else {
		NSSavePanel* openDialog = [NSSavePanel savePanel];
		if([openDialog runModal] == NSOKButton) {
			fileURL = [openDialog URL];
			if(fileURL.absoluteString.length > 0) {
				String result = [fileURL fileSystemRepresentation];
				return result;
			}
		}
	}
	return String();
}

void MacSystem::SetScreenDPI(float newDPI) {
    dpi = newDPI;
}

void MacSystem::SetAppIcon() {
	String iconPath = assets_path + "../../Logo.png";
	NSString* nsIconPath = [NSString stringWithCString:iconPath.ToCStr() encoding:[NSString defaultCStringEncoding]];
	NSImage* icon = [[NSImage alloc] initWithContentsOfFile: nsIconPath];
	[NSApp setApplicationIconImage:icon];
}
