//
//  MusicCore.h
//  iOS
//
//  Created by mouse on 4/25/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@interface MusicCore : NSObject

- (id)initWithFilename:(NSString*)filename;
- (void)play;
- (void)stop;
- (void)pause;

@end
