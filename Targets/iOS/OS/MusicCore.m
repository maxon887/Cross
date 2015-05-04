//
//  MusicCore.m
//  iOS
//
//  Created by mouse on 4/25/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#import "MusicCore.h"

@implementation MusicCore{
    AVPlayer* player;
}

- (id)initWithFilename:(NSString *)filename{
    self = [super init];
    if(self != nil){
        NSURL* url = [[NSURL alloc] initFileURLWithPath:filename];
        player = [[AVPlayer alloc] initWithURL:url];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerItemDidReachEnd:) name:AVPlayerItemDidPlayToEndTimeNotification object:[player currentItem]];
        [player addObserver:self forKeyPath:@"status" options:0 context:nil];
    }
    return self;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
    
    if (object == player && [keyPath isEqualToString:@"status"]) {
        if (player.status == AVPlayerStatusFailed) {
            NSLog(@"AVPlayer Failed");
            
        } else if (player.status == AVPlayerStatusReadyToPlay) {
            NSLog(@"AVPlayerStatusReadyToPlay");
            [player play];
            
            
        } else if (player.status == AVPlayerItemStatusUnknown) {
            NSLog(@"AVPlayer Unknown");
            
        }
    }
}

- (void)playerItemDidReachEnd:(NSNotification *)notification {
    
    //  code here to play next sound file
    
}

@end
