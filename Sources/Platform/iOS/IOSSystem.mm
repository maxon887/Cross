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
#include "IOSSystem.h"
#include "Audio.h"
#include "File.h"
#include "CommercialOS.h"

#include <sys/time.h>
#include <fstream>

IOSSystem::IOSSystem(){
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGFloat screenScale = [[UIScreen mainScreen] scale];
    S32 width = screenRect.size.width * screenScale;
    S32 height = screenRect.size.height * screenScale;
    SetWindowSize(width, height);
    commercial = new CommercialOS();
}

IOSSystem::~IOSSystem(){
    delete commercial;
    delete audio;
}

string IOSSystem::AssetsPath(){
    NSString* path = [[NSBundle mainBundle] bundlePath];
    string cPath = [path cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath + "/Assets/";
}

string IOSSystem::DataPath(){
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
    string cPath = [basePath cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath + "/";
}

void IOSSystem::Log(const char* str){
    NSLog(@"%@", [NSString stringWithFormat:@"%s", str]);
}

U64 IOSSystem::GetTime(){
    struct timeval ptv;
    gettimeofday(&ptv, NULL);
    return (ptv.tv_usec + ptv.tv_sec * 1000000LL);
}

Commercial* IOSSystem::GetCommercial(){
    return commercial;
}

void IOSSystem::RequestOrientation(cross::System::Orientation orientation){
    [UIViewController attemptRotationToDeviceOrientation];
}
