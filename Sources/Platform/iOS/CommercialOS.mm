//
//  CommercialOS.m
//  Jim the Snake
//
//  Created by mouse on 8/6/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#import "CommercialOS.h"
#import "CrossView.h"
#import "CrossViewController.h"
#import "IAPHelper.h"
#import <UIKit/UIKit.h>
#import <GoogleMobileAds/GoogleMobileAds.h>
#import <StoreKit/StoreKit.h>

static GADInterstitial* interstitial;
static IAPHelper* iapHelper;
static UIActivityIndicatorView* indicator;

CommercialOS::CommercialOS(){
    indicator = [[UIActivityIndicatorView alloc]initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    UIViewController* rootViewController = [CrossViewController getRootController];
    indicator.frame = CGRectMake(0.0, 0.0, 40.0, 40.0);
    indicator.transform = CGAffineTransformMakeScale(2.0, 2.0);
    indicator.center = rootViewController.view.center;
    [rootViewController.view addSubview:indicator];
    [indicator bringSubviewToFront:rootViewController.view];
    [UIApplication sharedApplication].networkActivityIndicatorVisible = TRUE;
    [UIView setAnimationsEnabled:NO];
    
    NSString* productID = @"com.cross.JimtheSnake.ads";
    iapHelper = [[IAPHelper alloc] initWithProductID:productID CommercialResultHander:^(int event){
        [indicator stopAnimating];
        CrossViewController* viewController = [CrossViewController getRootController];
        CrossView* crossView = viewController.CrossV;
        crossView.BlockInput = NO;
        
        if(event == EVENT_PURCHASE_FAILED){
            
        }
        
        Commercial::CommercialResult((Commercial::Event)event);
    }];
}

void CommercialOS::DownloadAd(){
    interstitial = [[GADInterstitial alloc] initWithAdUnitID:@"ca-app-pub-8147388388000575/9516738242"];
    GADRequest *request = [GADRequest request];
    request.testDevices = @[ @"4ae2c1e117e5b8d391130c539e7d855b" ];
    [interstitial loadRequest:request];
}

void CommercialOS::ShowAd(){
    if ([interstitial isReady]) {
        [interstitial presentFromRootViewController:[CrossViewController getRootController]];
    }
}

void CommercialOS::Purchase(){
    CrossViewController* viewController = [CrossViewController getRootController];
    CrossView* crossView = viewController.CrossV;
    crossView.BlockInput = YES;
    [indicator startAnimating];
    [iapHelper buy];
}

void CommercialOS::Restore(){
    CrossViewController* viewController = [CrossViewController getRootController];
    CrossView* crossView = viewController.CrossV;
    crossView.BlockInput = YES;
    [indicator startAnimating];
    [iapHelper restore];
}










