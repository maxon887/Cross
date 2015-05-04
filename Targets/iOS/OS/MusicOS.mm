//
//  MusicOS.cpp
//  iOS
//
//  Created by mouse on 4/25/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import "MusicCore.h"
#include "MusicOS.h"

static MusicCore* music_core;

MusicOS::MusicOS(const char* filename){
    music_core = [[MusicCore alloc]initWithFilename:[[NSString alloc] initWithCString:filename encoding:NSUTF8StringEncoding]];
    
}

void MusicOS::Play(){
    
}

void MusicOS::Stop(){
    
}

void MusicOS::Pause(){
    
}