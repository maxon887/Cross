//
//  CrossViewController.m
//  Cross++
//
//  Created by mouse on 6/25/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#import "CrossViewController.h"
#import "CrossView.h"

@implementation CrossViewController{
    IBOutlet CrossView* crossView;
}

- (void)didGoingBackground{
    [crossView didGoingBackground];
}

- (void)didGoingForeground{
    [crossView didGoingForeground];
}

@end
