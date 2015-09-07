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

#import "CrossView.h"
#import "Graphics.h"
#import "LauncherOS.h"
#import "Input.h"
#import "Cross.h"
#import <GoogleMobileAds/GoogleMobileAds.h>

@implementation CrossView{
    LauncherOS* launcher;
    Game* game;
    CGFloat screenScale;
    CADisplayLink* displayLink;
}

@synthesize BlockInput;

- (id)initWithCoder:(NSCoder *)aDecoder{
    self = [super initWithCoder:aDecoder];
    if(self){
        NSLog(@"initWithCoder");
        EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        self.context = context;
        self.enableSetNeedsDisplay = NO;
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
        
        screenScale = [[UIScreen mainScreen] scale];
        //ads test
        NSLog(@"Google Mobile Ads Version %@", [GADRequest sdkVersion]);
        BlockInput = false;
    }
    return self;
}

- (void)render:(CADisplayLink*)displayLink{
    [self display];
    if(!launcher){
        launcher = new LauncherOS();
        game = CrossMain(launcher);
        game->graphics = new Graphics(game);
        game->Start();
    }else{
        game->Update();
    }
}

- (void)didGoingBackground{
    game->Suspend();
    displayLink.paused = YES;
    [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
}

- (void)didGoingForeground{
    displayLink.paused = NO;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    if(!BlockInput){
        NSSet* touches = [event allTouches];
        if(touches.count == 1){
            game->input->input_state = true;
            UITouch *touch = [touches anyObject];
            CGPoint location = [touch locationInView:touch.view];
            game->input->input_loc.x = location.x * screenScale;
            game->input->input_loc.y = location.y * screenScale;
        }
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
    if(!BlockInput){
        NSSet* touches = [event allTouches];
        if(touches.count == 1){
            game->input->input_state = true;
            UITouch *touch = [touches anyObject];
            CGPoint location = [touch locationInView:touch.view];
            game->input->input_loc.x = location.x * screenScale;
            game->input->input_loc.y = location.y * screenScale;
        }
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    if(!BlockInput){
        game->input->input_state = false;
    }
}

@end
