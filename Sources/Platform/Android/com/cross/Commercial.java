package com.cross;

import android.app.Activity;
import android.util.Log;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

public class Commercial {
	
	class CommAdListener extends AdListener{
		@Override
		public void onAdLoaded() {
			Log.d("Cross++", "Ad load successfully");
		}
		@Override
		public void onAdFailedToLoad(int errorCode) {
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
	
	public static Activity mActivity;
	public static InterstitialAd mAd;
	
	
	public Commercial(Activity activity) {
		mActivity = activity;
	}
	
	public void DownloadAd() {
		Log.d("Cross++", "Java DownloadAd");
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Log.d("Cross++", "Java MainThread DownloadAd");
				mAd = new InterstitialAd(mActivity);
				mAd.setAdUnitId("ca-app-pub-8147388388000575/6331429441");
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
}
