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

#import <UIKit/UIKit.h>
#include "LauncherOS.h"

LauncherOS::LauncherOS(){
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGFloat screenScale = [[UIScreen mainScreen] scale];
    width = screenRect.size.width * screenScale;
    height = screenRect.size.height * screenScale;
    AudioOS::Init();
}

int LauncherOS::GetTargetWidth(){
    return width;
}

int LauncherOS::GetTargetHeight(){
    return height;
}

string LauncherOS::AssetsPath(){
    NSString* path = [[NSBundle mainBundle] bundlePath];
    string cPath = [path cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath + "/Assets/";
}

string LauncherOS::DataPath(){
    NSString* path = [[NSBundle mainBundle] resourcePath];
    string cPath = [path cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath + "/";
}

Music* LauncherOS::CreateMusic(string filename, bool loop){
    return new AudioOS(filename, loop, true);
}

Sound* LauncherOS::CreateSound(string filename, bool loop){
    return new AudioOS(filename, loop, false);
}

void LauncherOS::LogIt(string str){
    NSLog(@"%@", [NSString stringWithFormat:@"%s", str.c_str()]);
}


void LauncherOS::Exit(){
    exit(0);
}