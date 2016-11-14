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
#include "Audio.h"
#include "File.h"
#include "CommercialOS.h"

#include <sys/time.h>
#include <fstream>

LauncherOS::LauncherOS(){
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGFloat screenScale = [[UIScreen mainScreen] scale];
    width = screenRect.size.width * screenScale;
    height = screenRect.size.height * screenScale;
    commercial = new CommercialOS();
}

U32 LauncherOS::GetTargetWidth(){
    return width;
}

U32 LauncherOS::GetTargetHeight(){
    return height;
}

string LauncherOS::AssetsPath(){
    NSString* path = [[NSBundle mainBundle] bundlePath];
    string cPath = [path cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath + "/Default/";
}

string LauncherOS::DataPath(){
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
    string cPath = [basePath cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath + "/";
}

void LauncherOS::Log(const char* str){
    NSLog(@"%@", [NSString stringWithFormat:@"%s", str]);
}

U64 LauncherOS::GetTime(){
    struct timeval ptv;
    gettimeofday(&ptv, NULL);
    return (ptv.tv_usec + ptv.tv_sec * 1000000LL);
}

Commercial* LauncherOS::GetCommercial(){
    return commercial;
}

LauncherOS::~LauncherOS(){
    delete commercial;
    Audio::Release();
}
