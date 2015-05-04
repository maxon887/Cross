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

const char* LauncherOS::DataPath(){
    NSString* path = [[NSBundle mainBundle] resourcePath];
    const char* cPath = [path cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath;
}

void LauncherOS::LogIt(const char* str){
    NSLog(@"%@", [NSString stringWithFormat:@"%s", str]);
}

UIAlertView *alert;
void LauncherOS::ShowMessage(const char* title, const char *msg){
    NSLog(@"Message\n%s: %s",title, msg);
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSString* titleNS = [NSString stringWithFormat:@"%s", title];
        NSString* message = [NSString stringWithFormat:@"%s", msg];
        alert = [[UIAlertView alloc] initWithTitle:titleNS
                                                        message:message
                                                       delegate:nil
                                              cancelButtonTitle:nil
                                              otherButtonTitles:nil];
        [alert show];
    });
    [NSThread sleepForTimeInterval:20];
    [alert dismissWithClickedButtonIndex:0 animated:NO];
}

void LauncherOS::Exit(){
    exit(0);
}