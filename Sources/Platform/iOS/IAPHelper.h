//
//  IAPHelper.h
//  Jim the Snake
//
//  Created by mouse on 8/10/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

typedef void (^CommercialResultHander)(int event);
#define EVENT_PURCHASE_COMPLETE 2
#define EVENT_PURCHASE_CANCELED 3
#define EVENT_PURCHASE_FAILED   4


@interface IAPHelper : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver>

- (id)initWithProductID:(NSString*)productID CommercialResultHander:(CommercialResultHander)handler;
- (void)restore;
- (void)buy;

@end
