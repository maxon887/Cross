//
//  IAPHelper.m
//  Jim the Snake
//
//  Created by mouse on 8/10/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#import "IAPHelper.h"

@implementation IAPHelper{
    CommercialResultHander mCommercialResultHandler;
    SKProduct* mProduct;
    NSString* mProductID;
}

- (id)initWithProductID:(NSString*)productID CommercialResultHander:(CommercialResultHander)handler{
    self = [super init];
    if(self){
        mProductID = productID;
        mCommercialResultHandler = handler;
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    }
    return self;
}

- (void)productsRequest:(SKProductsRequest*)request didReceiveResponse:(SKProductsResponse *)response{
   // NSLog(@"Loaded list of products:");
    //mProductRequest = nil;
    NSArray* skProducts = response.products;
    for(SKProduct* skProduct in skProducts){
        NSLog(@"Found product: %@ %@ %0.2f",
              skProduct.productIdentifier,
              skProduct.localizedTitle,
              skProduct.price.floatValue);
    }
    if(skProducts.count > 0){
        mProduct = [skProducts objectAtIndex:0];
        NSLog(@"Buying %@", mProduct.productIdentifier);
        SKPayment* payment = [SKPayment paymentWithProduct:mProduct];
        [[SKPaymentQueue defaultQueue] addPayment:payment];
    }else{
        mCommercialResultHandler(EVENT_PURCHASE_FAILED);
    }
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error{
    mCommercialResultHandler(EVENT_PURCHASE_FAILED);
}


- (void)buy{
    [self requestProduct];
}

- (void)requestProduct{
    SKProductsRequest* productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithObjects:mProductID, nil]];
    productRequest.delegate = self;
    [productRequest start];
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions{
    for (SKPaymentTransaction * transaction in transactions) {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
            default:
                break;
        }
    };
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"completeTransaction...");
    mCommercialResultHandler(EVENT_PURCHASE_COMPLITE);
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"restoreTransaction...");
    mCommercialResultHandler(EVENT_PURCHASE_COMPLITE);
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"failedTransaction...");
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
        NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
        mCommercialResultHandler(EVENT_PURCHASE_FAILED);
    }
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

@end
