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
#import "Cross.h"
#import "CrossView.h"
#import "LauncherOS.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Input.h"
//#import <GoogleMobileAds/GoogleMobileAds.h>

@implementation CrossView{
    BOOL disabled;
    CGFloat screenScale;
    CADisplayLink* displayLink;
}

@synthesize BlockInput;

- (id)initWithCoder:(NSCoder *)aDecoder{
    self = [super initWithCoder:aDecoder];
    if(self){
        NSLog(@"initWithCoder");
        disabled = false;
        EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        self.context = context;
        self.drawableDepthFormat = GLKViewDrawableDepthFormat16;
        self.enableSetNeedsDisplay = NO;
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
        
        screenScale = [[UIScreen mainScreen] scale];
        //self.multipleTouchEnabled = YES;
        //ads test
        //NSLog(@"Google Mobile Ads Version %@", [GADRequest sdkVersion]);
        BlockInput = false;
    }
    return self;
}

- (void)render:(CADisplayLink*)displayLink{
    [self display];
    try{
        if(!launcher){
            launcher = new LauncherOS();
            game = CrossMain(launcher);
            Audio::Init();
            gfxGL = new GraphicsGL();
            gfx2D = new Graphics2D();
            gfx3D = new Graphics3D();
            game->Start();
            game->SetScreen(game->GetStartScreen());
        }else{
            if(!disabled){
                game->Update();
            }
        }
    } catch(Exception &exc) {
        string msg = string(exc.message) +
        +"\nFile: " + string(exc.filename) +
        +"\nLine: " + to_string(exc.line);
        NSLog(@"%@", [NSString stringWithFormat:@"\nUnhandled Exception:\n\t%s", msg.c_str()]);
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Unhandled Exception"
                                                        message:[NSString stringWithFormat:@"%s", msg.c_str()]
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
        disabled = true;
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

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event{
    [super touchesBegan:touches withEvent:event];
    if(!BlockInput){
        NSSet* touches = [event allTouches];
        UITouch* touch = [touches anyObject];
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        TRIGGER_EVENT(input->TargetActionDown, x, y, 0);
    }
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event{
    [super touchesMoved:touches withEvent:event];
    if(!BlockInput){
        NSSet* touches = [event allTouches];
        UITouch* touch = [touches anyObject];
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        TRIGGER_EVENT(input->TargetActionMove, x, y, 0);
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event{
    [super touchesEnded:touches withEvent:event];
    if(!BlockInput){
        NSSet* touches = [event allTouches];
        UITouch* touch = [touches anyObject];
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        TRIGGER_EVENT(input->TargetActionUp, x, y, 0);
    }
}

@end
