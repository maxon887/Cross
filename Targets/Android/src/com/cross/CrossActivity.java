package com.cross;
import org.fmod.FMOD;

import android.app.NativeActivity;
import android.os.Bundle;


public class CrossActivity extends NativeActivity 
							implements Runnable{
	
	private native void main();
	private Thread mThread;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		FMOD.init(this);
        mThread = new Thread(this, "Example Main");
        mThread.start();
       
	}
	
	static{
		System.loadLibrary("c++_shared");
		System.loadLibrary("fmodL");
		System.loadLibrary("cross");
	}

	@Override
	public void run() {
        main();
	}
}
