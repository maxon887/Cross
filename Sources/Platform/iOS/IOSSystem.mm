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
#import "CrossViewController.h"
#include "IOSSystem.h"
#include "Audio.h"
#include "File.h"

#include <sys/time.h>
#include <fstream>

@interface MessageboxDelegate : NSObject

@end

@implementation MessageboxDelegate

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
    [CrossViewController Instance].CrossPaused = NO;
}

@end

IOSSystem::IOSSystem(){
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGFloat screenScale = [[UIScreen mainScreen] scale];
    S32 width = screenRect.size.width * screenScale;
    S32 height = screenRect.size.height * screenScale;
    SetWindowSize(width, height);
}

IOSSystem::~IOSSystem(){
    delete audio;
}

void IOSSystem::Log(const char* str){
    NSLog(@"%@", [NSString stringWithFormat:@"%s", str]);
}

String IOSSystem::AssetsPath(){
    NSString* path = [[NSBundle mainBundle] bundlePath];
    String cPath = [path cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath + "/Assets/";
}

String IOSSystem::DataPath(){
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
    String cPath = [basePath cStringUsingEncoding:[NSString defaultCStringEncoding]];
    return cPath + "/";
}

U64 IOSSystem::GetTime(){
    struct timeval ptv;
    gettimeofday(&ptv, NULL);
    return (ptv.tv_usec + ptv.tv_sec * 1000000LL);
}

float IOSSystem::GetScreenDPI() {
    float scale = 1;
    if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
        scale = [[UIScreen mainScreen] scale];
    }
    return 160 * scale;
}

void IOSSystem::RequestOrientation(cross::System::Orientation orientation){
    [UIViewController attemptRotationToDeviceOrientation];
}

bool IOSSystem::IsMobile() {
    return true;
}

void IOSSystem::Messagebox(const String& title, const String& message){
    CrossViewController* viewController = [CrossViewController Instance];
    viewController.CrossPaused = YES;
    
    MessageboxDelegate* del = [[MessageboxDelegate alloc] init];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:[NSString stringWithFormat:@"%s", title.ToCStr()]
                                                    message:[NSString stringWithFormat:@"%s", message.ToCStr()]
                                                   delegate:del
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
    while(viewController.CrossPaused){
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
    }
}

