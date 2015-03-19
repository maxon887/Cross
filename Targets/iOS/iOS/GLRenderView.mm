//
//  GLRenderView.m
//  iOSLauncher
//
//  Created by mouse on 3/3/15.
//  Copyright (c) 2015 mouse. All rights reserved.
//

#import "GLRenderView.h"
#import "Graphics.h"

@implementation GLRenderView{
    Graphics* gfx;
}

- (id)initWithCoder:(NSCoder *)aDecoder{
    self = [super initWithCoder:aDecoder];
    
    NSLog(@"initWithCoder");
    EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    self.context = context;
    self.enableSetNeedsDisplay = NO;
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    gfx = new Graphics();
    gfx->Init();
    return self;
}

- (void)render:(CADisplayLink*)displayLink{
    [self display];
    gfx->Test();
}

@end
