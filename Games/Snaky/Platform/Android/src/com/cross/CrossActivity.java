package com.cross;
import org.fmod.FMOD;

import android.app.NativeActivity;

public class CrossActivity extends NativeActivity 
							implements Runnable{
	
	static{
		System.loadLibrary("c++_shared");
		System.loadLibrary("fmodL");
		System.loadLibrary("cross");
	}
	
	private Thread mThread;
	
	private native void InitAudio();
	private native void ReleaseAudio();
	
	@Override
	protected void onResume() {
		FMOD.init(this);
        mThread = new Thread(this, "Audio thread");
        mThread.start();
		super.onResume();
	}
	
	@Override
	protected void onPause() {
		ReleaseAudio();
		FMOD.close();
		super.onPause();
	}
	
	@Override
	public void run() {
		InitAudio();
	}
}