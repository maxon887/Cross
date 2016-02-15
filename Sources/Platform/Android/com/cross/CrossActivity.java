package com.cross;

import org.fmod.FMOD;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class CrossActivity extends Activity{
	private static Cross cross;
	private static boolean cross_initialized = false;
	private static Commercial commercial;
	private static AssetManager asset_manager;
	private GLSurfaceView renderer;
	
	static{
		System.loadLibrary("c++_shared");
		System.loadLibrary("fmod");
		System.loadLibrary("cross");
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.d("Cross++", "Java onCreate");
		super.onCreate(savedInstanceState);
		cross_initialized = false;
		//renderer = new CrossRenderer(this);
		renderer = new GL20Renderer(this);
		try{
			renderer.setPreserveEGLContextOnPause(true);
		}catch(Throwable ex){
			Log.d("Cross++", "Java setPreserveEGLContextOnPause exception");
		}
		setContentView(renderer);
		try{
			//commercial = new Commercial(this);
		}catch(Throwable ex){
			Log.d("Cross++", "Java can not create commercial");
			commercial = null;
		}
		//rate it
	    RateThisApp.onStart(this);
	    RateThisApp.showRateDialogIfNeeded(this);
	}
	
	@Override
	protected void onPause() {
		Log.d("Cross++", "Java onPause");
		super.onPause();
		if(cross_initialized){
			cross.Suspend();
			renderer.onPause();
		}
	}
	
	@Override
	protected void onResume() {
		Log.d("Cross++", "Java onResume");
		super.onResume();
		renderer.onResume();
		if(cross_initialized) {
			cross.Resume();
		}
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if(cross_initialized){
			switch (event.getAction()) {
			case MotionEvent.ACTION_DOWN:
				cross.ActionDown(event.getX(), event.getY());
				return true;
			case MotionEvent.ACTION_MOVE:
				cross.ActionMove(event.getX(), event.getY());
				return true;
			case MotionEvent.ACTION_UP:
				cross.ActionUp(event.getX(), event.getY());
				return true;
			}
		}
		return false;
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if(cross_initialized){
			switch (keyCode) {
			case KeyEvent.KEYCODE_BACK:
				cross.PressKey(Cross.KEY_BACK);
				return true;
			case KeyEvent.KEYCODE_MENU:
				cross.PressKey(Cross.KEY_OPTIONS);
				return true;
			}
		}
		return false;
	}
	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		if(cross_initialized){
			switch(keyCode){
			case KeyEvent.KEYCODE_BACK:
				cross.ReleaseKey(Cross.KEY_BACK);
				return true;
			case KeyEvent.KEYCODE_MENU:
				cross.ReleaseKey(Cross.KEY_OPTIONS);
				return true;
			}
		}
		return false;
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if(commercial != null){
			//commercial.HandleActivityResult(requestCode, resultCode, data);
		}
	}
	
	public void Init(int width, int height){
		Log.d("Cross++", "Java Init OpenGL");
		if(!cross_initialized){
			FMOD.init(this);
			String dataPath =  getFilesDir().getPath();
			cross = new Cross();
			asset_manager = getResources().getAssets();
			cross.Init(width, height, dataPath, asset_manager, this);
			if(commercial != null){
				cross.InitialCommercial(commercial);
			}
			cross.Start();
			cross_initialized = true;
		}else{
			Log.d("Cross++", "Java initialization unexpected behaviour");
		}
	}
	
	public void PromtToExit() {
		Log.d("Cross++", "Java PromtToExit");
	    new AlertDialog.Builder(CrossActivity.this)
        .setIcon(android.R.drawable.ic_dialog_alert)
        .setTitle("Confirm Exit")
        .setMessage("Do you want to quit the game?")
        .setPositiveButton("Yes", new DialogInterface.OnClickListener()   {
	        @Override
	        public void onClick(DialogInterface dialog, int which) {
	        	ReleaseCross();
	        	System.exit(0);
	        }
        })
        .setNegativeButton("No", null)
        .show();
	}
	
	public void SendCommertialResult(int event) {
		if(cross != null) {
			cross.CommertialResult(event);
		} else {
			Log.d("Cross++", "Can't send CommertialResult. cross = null");
		}
	}
	
	public void Update(){
		if(cross_initialized){
			cross.Update();
		}
	}
	
	public void ReleaseCross(){
		if(cross_initialized){
			cross_initialized = false;
			cross.Release();
			cross = null;
			FMOD.close();
		}
	}
}