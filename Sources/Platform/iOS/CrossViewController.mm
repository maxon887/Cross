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
#import "CrossViewController.h"
#include "Cross.h"
#include "IOSSystem.h"
#include "Internals/Audio.h"
#include "Internals/GraphicsGL.h"
#include "Input.h"
#include "Game.h"
#include "Config.h"

#define MAX_TOUCHES 11

using namespace cross;

CrossViewController* instance = nil;

@interface CrossViewController()

- (S32)getTouchID:(UITouch*) touch;
- (S32)addTouchID:(UITouch*) touch;

@end

@implementation CrossViewController{
    CGFloat screenScale;
    UITouch* touchIDs[MAX_TOUCHES];
}

@synthesize CrossPaused;

+ (CrossViewController*)Instance{
    return instance;
}

- (void)viewDidLoad{
    [super viewDidLoad];
    instance = self;
    self.preferredFramesPerSecond = 60;
    self.view.multipleTouchEnabled = YES;
    for(int i = 0; i < MAX_TOUCHES; ++i){
        touchIDs[i] = NULL;
    }
    EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:context];
    GLKView* view = (GLKView*)self.view;
    view.context = context;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat16;
    view.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    screenScale = [[UIScreen mainScreen] scale];
    CrossPaused = NO;
}

- (void)update{
    if(!CrossPaused){
		GLKView* view = (GLKView*)self.view;
        if(!cross::os){
			cross::os = new IOSSystem();
			cross::os->SetWindowSize((S32)view.drawableWidth, (S32)view.drawableHeight);
			
            game = CrossMain();
            audio = new Audio();
            gfxGL = new GraphicsGL();
            game->Start();
        }
		//detecting orientation changed
		if(view.drawableWidth != cross::os->GetWindowWidth()){
			cross::os->SetWindowSize((S32)view.drawableWidth, (S32)view.drawableHeight);
		}
		
        game->EngineUpdate();
    }
}

- (void)dealloc{
    NSLog(@"Dealloc");
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    for (UITouch* touch in touches) {
        S32 touchID = [self addTouchID:touch];
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        input->TargetActionDownHandle(x, y, touchID);
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        S32 touchID = [self getTouchID:touch];
        touchIDs[touchID] = NULL;
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        input->TargetActionUpHandle(x, y, touchID);
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        S32 touchID = [self getTouchID:touch];
        touchIDs[touchID] = NULL;
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        input->TargetActionUpHandle(x, y, touchID);
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        S32 touchID = [self getTouchID:touch];
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        input->TargetActionMoveHandle(x, y, touchID);
    }
}

- (S32)getTouchID:(UITouch*) touch{
    for(int i = 0; i < MAX_TOUCHES; ++i){
        if(touchIDs[i] == touch){
            return i;
        }
    }
    NSLog(@"Can't find touch id");
    return -1;
}

- (S32)addTouchID:(UITouch*) touch{
    for(int i = 0; i < MAX_TOUCHES; ++i){
        if(!touchIDs[i]){
            touchIDs[i] = touch;
            return i;
        }
    }
    NSLog(@"Can't add new touch");
    return -1;
}


@end
