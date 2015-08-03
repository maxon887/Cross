package com.cross;

import android.content.Context;
import android.util.Log;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

public class Commercial {
	InterstitialAd mAd;
	
	public Commercial(Context context) {
		mAd = new InterstitialAd(context);
		mAd.setAdUnitId("ca-app-pub-8147388388000575/6331429441");
	}
	
	public void DownloadAd() {
		Log.d("Cross++", "Java DownloadAd");
		AdRequest adRequest = new AdRequest.Builder().build();
		mAd.loadAd(adRequest);
	}
	
	public void ShowAd() {
		Log.d("Cross++", "Java ShowAd");
		if(mAd.isLoaded()) {
			mAd.show();
		}
	}
}
