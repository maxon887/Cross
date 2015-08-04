package com.cross;

import android.app.Activity;
import android.util.Log;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

public class Commercial {
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
