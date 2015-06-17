package com.cross;

import android.content.res.AssetManager;

public class Cross {

	public native void Init(int width, int height, String dataPath, AssetManager manager);
	public native void Update();
	public native void Suspend();
	public native void Release();
	
	public native void SetInputState(boolean state);
	public native void SetInputLoc(float x, float y);
	public native void SetKeyState(boolean state);
	public native void SetKeyKey(int key);
	
	public native void InitAudio();
}
