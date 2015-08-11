package com.cross;

import android.content.res.AssetManager;

public class Cross {
	public static final int	KEY_UNDEFINED 	= 0;
	public static final int	KEY_PAUSE		= 1;
	public static final int	KEY_BACK		= 2;
	public static final int	KEY_OPTIONS		= 3;

	public native void Init(int w, int h, String dataPath, AssetManager manager, CrossActivity crossActivity);
	public native void Start();
	public native void Release();
	public native void Update();
	public native void Suspend();
	public native void Resume();
	
	public native void SetInputState(boolean state);
	public native void SetInputLoc(float x, float y);
	public native void SetKeyState(boolean state);
	public native void SetKeyKey(int key);
	
	public native void InitialCommercial(Commercial comm);
	public native void CommertialResult(int event);
}
