package com.cross;

import android.content.Intent;
import android.util.Log;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

import com.mouse.billing.IabHelper;
import com.mouse.billing.IabResult;
import com.mouse.billing.Purchase;
import com.mouse.billing.IabHelper.OnIabPurchaseFinishedListener;

public class Commercial {
	public static final int EVENT_AD_LOADED 		= 0;
	public static final int EVENT_AD_LOAD_FAILED 	= 1;
	public static final int EVENT_PURCHASE_COMPLITE = 2;
	public static final int EVENT_PURCHASE_CANCELED = 3;
	public static final int EVENT_PURCHASE_FAILED 	= 4;
	
	class CommAdListener extends AdListener{
		@Override
		public void onAdLoaded() {
			Log.d("Cross++", "Ad load successfully");
			mActivity.SendCommertialResult(EVENT_AD_LOADED);
		}
		@Override
		public void onAdFailedToLoad(int errorCode) {
			mActivity.SendCommertialResult(EVENT_AD_LOAD_FAILED);
			String message = "Ad failed to load: ";
		    switch(errorCode) {
		      case AdRequest.ERROR_CODE_INTERNAL_ERROR:
		    	message += "Internal error";
		        break;
		      case AdRequest.ERROR_CODE_INVALID_REQUEST:
		    	message += "Invalid request";
		        break;
		      case AdRequest.ERROR_CODE_NETWORK_ERROR:
		    	message += "Network Error";
		        break;
		      case AdRequest.ERROR_CODE_NO_FILL:
		    	message += "No fill";
		        break;
		    }
			Log.d("Cross++", message);
		}
		@Override
		public void onAdOpened() {
			Log.d("Cross++", "Ad showed!");
		}
		@Override
		public void onAdClosed() {
		}
	}
	
	class PurchaseFinishedListener implements OnIabPurchaseFinishedListener{
		@Override
		public void onIabPurchaseFinished(IabResult result, Purchase info) {
			if (!result.isFailure()) {
				mActivity.SendCommertialResult(EVENT_PURCHASE_COMPLITE);
			}else{
				if(result.getResponse() == IabHelper.BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED){
					mActivity.SendCommertialResult(EVENT_PURCHASE_COMPLITE);
				}else {
					Log.d("Cross++", "Can't purchase product: " + result);
					mActivity.SendCommertialResult(EVENT_PURCHASE_FAILED);
				}
			}
		}
	}
	
	private static final String app_key = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAhg/KgTQjD4eTqTWUJ5iV0G7RcfdiiZ+uEJT+RhuBcx2TguoIkASZKn1QLSflrJY5nYB7UDyt5Vh2AtSLIdP6EWgOybcvohPBYjuXrwsOKWYUadfk1KCebV/565KH58Ex77M5MDDdYV8/WjIm2KOKEqD4bcoBsNdRLpvREXlGLPlJrRyQDVd3hA6b4pJ7PNFGzwRibCo7Ix63UPhTCHpkNgqJXwUc0s/Bx/0vu9qY/Pxwo2+tp5280+J1OZyx1khjq31s37MWAV+SuIu8O/PnG5uXq3byTclsSUpP7oQgyqUloqMhtlcW4FzGkh89o1FWbimzLSwghZ6xKGp8mUAw0QIDAQAB";
	private static IabHelper mHelper;
	private static CrossActivity mActivity;
	private static InterstitialAd mAd;
	
	public Commercial(CrossActivity activity) {
		mActivity = activity;
		mHelper = new IabHelper(activity, app_key);
		mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
			@Override
			public void onIabSetupFinished(IabResult result) {
				if(!result.isSuccess()) {
			         Log.d("Cross++", "Problem setting up In-app Billing: " + result);
				}
			}
		});
	}
	
	public void DownloadAd() {
		Log.d("Cross++", "Java DownloadAd");
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mAd = new InterstitialAd(mActivity);
				mAd.setAdUnitId("ca-app-pub-2859596256803284/4602746658");
				AdRequest adRequest = new AdRequest.Builder().build();
				mAd.setAdListener(new CommAdListener());
				mAd.loadAd(adRequest);
			}
		});
	}
	
	public void ShowAd() {
		Log.d("Cross++", "Java ShowAd");
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if(mAd.isLoaded()) {
					mAd.show();
				}
			}
		});
	}
	
	public void Purchase() {
		Log.d("Cross++", "Java Purchase");
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					mHelper.launchPurchaseFlow(mActivity, "ads", 69, new PurchaseFinishedListener(), "Jim the Snake");	
				} catch(Throwable ex) {
					Log.d("Cross++", "Java: " + ex.getMessage());
				}
			}
		});
	}
	
	public void HandleActivityResult(int requestCode, int resultCode, Intent data) {
		mHelper.handleActivityResult(requestCode, resultCode, data);
	}
}
