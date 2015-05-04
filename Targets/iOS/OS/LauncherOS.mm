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
}

int LauncherOS::GetTargetWidth(){
    return width;
}

int LauncherOS::GetTargetHeight(){
    return height;
}

const char* LauncherOS::AssetsPath(){
    NSString* path = [[NSBundle mainBundle] resourcePath];
    const char* cPath = [path cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath;
}

const char* LauncherOS::DataPath(){
    NSString* path = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    const char* cPath = [path cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath;
}

void LauncherOS::LogIt(const char* str){
    NSLog(@"%@", [NSString stringWithFormat:@"%s", str]);
}

Sound* LauncherOS::CreateSound(const char *filename, bool loop){
    return NULL;
}

Music* LauncherOS::CreateMusic(const char *filename, bool loop){
    sprintf(str_buff, "%s/%s", AssetsPath(), filename);
    return new MusicOS(str_buff);
}




