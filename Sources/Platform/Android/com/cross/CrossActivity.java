package com.cross;

import org.fmod.FMOD;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class CrossActivity extends Activity{
	public static Cross cross;
	public static Commercial commercial;
	public static AssetManager asset_manager;
	private CrossRenderer renderer;
	
	static{
		System.loadLibrary("c++_shared");
		System.loadLibrary("fmod");
		System.loadLibrary("cross");
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.d("Cross++", "Java onCreate");
		super.onCreate(savedInstanceState);
		renderer = new CrossRenderer(this);
		renderer.setPreserveEGLContextOnPause(true);
		setContentView(renderer);
		commercial = new Commercial(this);
	}
	
	@Override
	protected void onPause() {
		Log.d("Cross++", "Java onPause");
		super.onPause();
		renderer.onPause();
		if(cross != null){
			cross.Suspend();
		}
	}
	
	@Override
	protected void onResume() {
		Log.d("Cross++", "Java onResume");
		super.onResume();
		renderer.onResume();
		if(cross != null) {
			cross.Resume();
		}
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
		case MotionEvent.ACTION_MOVE:
			cross.SetInputState(true);
			cross.SetInputLoc(event.getX(), event.getY());
			return true;
		case MotionEvent.ACTION_UP:
			cross.SetInputState(false);
			return true;
		}
		return false;
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		switch (keyCode) {
		case KeyEvent.KEYCODE_BACK:
			cross.SetKeyState(true);
			cross.SetKeyKey(1);	//1 means pause key
			return true;
		case KeyEvent.KEYCODE_MENU:
			cross.SetKeyState(true);
			cross.SetKeyKey(1);
			return true;
		}
		return false;
	}
	
	public void Init(int width, int height){
		Log.d("Cross++", "Java Init");
		if(cross == null){
			FMOD.init(this);
			String dataPath =  getFilesDir().getPath();
			cross = new Cross();
			asset_manager = getResources().getAssets();
			cross.Init(width, height, dataPath, asset_manager);
			//commercial = new Commercial(this);
			cross.InitialCommercial(commercial);
		}
	}
	
	public void Update(){
		cross.Update();
	}	
	
	public void ReleaseCross(){
		if(cross != null){
			cross.Release();
			FMOD.close();
			cross = null;
		}
	}
}