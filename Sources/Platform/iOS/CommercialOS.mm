//
//  CommercialOS.m
//  Jim the Snake
//
//  Created by mouse on 8/6/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#import "CommercialOS.h"
#import "CrossViewController.h"
#import <UIKit/UIKit.h>
#import <GoogleMobileAds/GoogleMobileAds.h>

static GADInterstitial *interstitial;

void CommercialOS::DownloadAd(){
    interstitial = [[GADInterstitial alloc] initWithAdUnitID:@"ca-app-pub-8147388388000575/9516738242"];
    GADRequest *request = [GADRequest request];
    request.testDevices = @[@"1c58e9df9dfdf889f4be1cc33f841ff6"];
    [interstitial loadRequest:request];
}

void CommercialOS::ShowAd(){
    if ([interstitial isReady]) {
        [interstitial presentFromRootViewController:[CrossViewController getRootController]];
    }
}