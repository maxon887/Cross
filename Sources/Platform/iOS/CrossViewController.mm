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
#include "LauncherOS.h"
#include "Audio.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Input.h"
#include "Game.h"
#include "TouchCounter.hpp"

@implementation CrossViewController{
    CGFloat screenScale;
    BOOL paused;
}

- (void)viewDidLoad{
    [super viewDidLoad];
    self.preferredFramesPerSecond = 60;
    self.view.multipleTouchEnabled = YES;
    EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:context];
    GLKView* view = (GLKView*)self.view;
    view.context = context;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat16;
    view.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    screenScale = [[UIScreen mainScreen] scale];
    
    try{
        launcher = new LauncherOS();
        game = CrossMain(launcher);
        Audio::Init();
        gfxGL = new GraphicsGL();
        gfx2D = new Graphics2D();
        gfx3D = new Graphics3D();
        game->Start();
        game->SetScreen(game->GetStartScreen());
    } catch(Exception &exc){
        paused = YES;
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
    }
}

- (void)update{
    try{
        if(!paused){
            game->Update();
        }
    } catch(Exception &exc) {
        paused = YES;
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
    }
}

- (void)dealloc{
    NSLog(@"Dealloc");
}

// Handles the start of a touch
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch *touch in touches)
    {
        int fingerID = GetFingerTrackIDByTouch((__bridge void *)touch);
        
        if (fingerID == -1)
        {
            fingerID = AddNewTouch((__bridge void *)touch);
        } else
        {
            continue;
        }
        
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        
        TRIGGER_EVENT(input->TargetActionDown, x, y, fingerID);
    }
    
    
#ifdef _DEBUG
    //LogMsg("%d touches active", GetTouchesActive());
#endif
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Enumerate through all the touch objects.
    for (UITouch *touch in touches)
    {
        //found a touch.  Is it already on our list?
        int fingerID = GetFingerTrackIDByTouch((__bridge void *)touch);
        if (fingerID != -1)
        {
            g_touchTracker[fingerID].m_touchPointer = NULL; //clear it
        } else
        {
            //wasn't on our list
            continue;
        }
        
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        
        TRIGGER_EVENT(input->TargetActionUp, x, y, fingerID);
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Enumerate through all the touch objects.
    for (UITouch *touch in touches)
    {
        //found a touch.  Is it already on our list?
        int fingerID = GetFingerTrackIDByTouch((__bridge void *)touch);
        if (fingerID != -1)
        {
            g_touchTracker[fingerID].m_touchPointer = NULL; //clear it
        } else
        {
            //wasn't on our list
            continue;
        }
        
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        
        TRIGGER_EVENT(input->TargetActionUp, x, y, fingerID);
    }
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Enumerate through all the touch objects.
    for (UITouch *touch in touches)
    {
        
        //found a touch.  Is it already on our list?
        int fingerID = GetFingerTrackIDByTouch((__bridge void *)touch);
        if (fingerID != -1)
        {
            //found it
        } else
        {
            continue;
        }
        
        CGPoint pos = [touch locationInView:touch.view];
        float x = pos.x * screenScale;
        float y = pos.y * screenScale;
        
        TRIGGER_EVENT(input->TargetActionMove, x, y, fingerID);
    }
}

@end
