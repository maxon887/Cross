//
//  GLRenderView.h
//  iOSLauncher
//
//  Created by mouse on 3/3/15.
//  Copyright (c) 2015 mouse. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface GLRenderView : GLKView

- (id)initWithCoder:(NSCoder *)aDecoder;
- (void)render:(CADisplayLink*)displayLink;

@end
