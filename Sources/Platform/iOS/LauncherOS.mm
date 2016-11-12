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
#include "File.h"
#include "CommercialOS.h"
#include "LauncherOS.h"

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
    return cPath + "/Assets/Default/";
}

string LauncherOS::DataPath(){
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
    string cPath = [basePath cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath + "/";
}

void LauncherOS::LogIt(const string& str){
    NSLog(@"%@", [NSString stringWithFormat:@"%s", str.c_str()]);
}

void LauncherOS::LogIt(const char* formatStr, ...){
    va_list params;
    char buffer[1024];
    va_start(params, formatStr);
    vsprintf(buffer, formatStr, params);
    NSLog(@"%@", [NSString stringWithFormat:@"%s", buffer]);
    va_end(params);
}

File* LauncherOS::LoadFile(const string& filename){
    File* file = new File();
    file->name = filename;
    string filePath = AssetsPath() + filename;
    ifstream fileStream(filePath, istream::binary);
    if(fileStream.is_open()){
        fileStream.seekg(0, fileStream.end);
        file->size = (size_t)fileStream.tellg();
        fileStream.seekg(0, fileStream.beg);
        file->data = new Byte[file->size];
        //ZeroMemory(file->data, file->size);
        fileStream.read((char*)file->data, file->size);
        fileStream.close();
        return file;
    } else {
        throw CrossException("Cannot open file %s", file->name.c_str());
    }
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




