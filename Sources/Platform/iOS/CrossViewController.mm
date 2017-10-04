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
#include "Audio.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
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
        if(!sys){
            sys = new IOSSystem();
            game = CrossMain();
            audio = new Audio();
            gfxGL = new GraphicsGL();
            gfx2D = new Graphics2D();
            gfx3D = new Graphics3D();
            game->Start();
            game->SetScreen(game->GetStartScreen());
        }
        game->EngineUpdate();
    }
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator{
    if(system){
        CGFloat scale = [[UIScreen mainScreen] scale];
        S32 width = size.width * scale;
        S32 height = size.height * scale;
        system->SetWindowSize(width, height);
    }
}

- (NSUInteger)supportedInterfaceOrientations{
    if(config){
        switch(cross::config->GetOrientation()){
            case System::Orientation::AUTO:
                return UIInterfaceOrientationMaskAll;
            case System::Orientation::LANDSCAPE:
                return UIInterfaceOrientationMaskLandscape;
            case System::Orientation::PORTRAIT:
                return UIInterfaceOrientationMaskPortrait;
            default:
                CROSS_RETURN(false, 0, "Unknown screen orientation");
        }
    }
    return UIInterfaceOrientationMaskAll;
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
        input->TargetActionDown(x, y, touchID);
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        S32 touchID = [self getTouchID:touch];
        touchIDs[touchID] = NULL;
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        input->TargetActionUp(x, y, touchID);
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        S32 touchID = [self getTouchID:touch];
        touchIDs[touchID] = NULL;
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        input->TargetActionUp(x, y, touchID);
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        S32 touchID = [self getTouchID:touch];
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        input->TargetActionMove(x, y, touchID);
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
