//
//  CrossViewController.h
//  Cross++
//
//  Created by mouse on 6/25/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CrossView.h"

@interface CrossViewController : UIViewController

@property (weak, nonatomic) IBOutlet CrossView* CrossV;

+ (CrossViewController*)getRootController;

- (void)didGoingBackground;
- (void)didGoingForeground;

@end
