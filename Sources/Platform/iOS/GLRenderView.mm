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

#import <time.h>
#import "GLRenderView.h"
#import "Graphics.h"
#import "LauncherOS.h"
#import "Input.h"
#import "SnakyGame.h"

@implementation GLRenderView{
    LauncherOS* launcher;
    Game* game;

    CGFloat screenScale;
}

- (id)initWithCoder:(NSCoder *)aDecoder{
    self = [super initWithCoder:aDecoder];
    
    NSLog(@"initWithCoder");
    EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    self.context = context;
    self.enableSetNeedsDisplay = NO;
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    
    screenScale = [[UIScreen mainScreen] scale];
    return self;
}

- (void)render:(CADisplayLink*)displayLink{
    [self display];
    if(!launcher){
        launcher = new LauncherOS();
        game = new SnakyGame(launcher);
        Graphics* graphics = new Graphics(game);
        game->graphics = graphics;
        game->Start();
    }
    game->Update();
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    game->input->input_state = true;
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    game->input->input_loc.x = location.x * screenScale;
    game->input->input_loc.y = location.y * screenScale;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    game->input->input_loc.x = location.x * screenScale;
    game->input->input_loc.y = location.y * screenScale;
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    game->input->input_state = false;
}

@end
