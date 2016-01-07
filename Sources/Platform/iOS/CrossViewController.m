//
//  CrossViewController.m
//  Cross++
//
//  Created by mouse on 6/25/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#import "CrossViewController.h"

static CrossViewController* rootViewController;

@implementation CrossViewController{
    //IBOutlet CrossView* crossView;
}

@synthesize CrossV;

- (void)viewDidLoad{
    [super viewDidLoad];
    rootViewController = self;
}

+ (CrossViewController*)getRootController{
    return rootViewController;
}

- (void)didGoingBackground{
    [CrossV didGoingBackground];
}

- (void)didGoingForeground{
    [CrossV didGoingForeground];
}

@end
