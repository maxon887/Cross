package com.cross;

import org.fmod.FMOD;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
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
		//rate it
	    RateThisApp.onStart(this);
	    RateThisApp.showRateDialogIfNeeded(this);
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
			cross.SetKeyKey(Cross.KEY_BACK);
			return true;
		case KeyEvent.KEYCODE_MENU:
			cross.SetKeyState(true);
			cross.SetKeyKey(Cross.KEY_OPTIONS);
			return true;
		}
		return false;
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		commercial.HandleActivityResult(requestCode, resultCode, data);
	}
	
	public void Init(int width, int height){
		Log.d("Cross++", "Java Init");
		if(cross == null){
			FMOD.init(this);
			String dataPath =  getFilesDir().getPath();
			cross = new Cross();
			asset_manager = getResources().getAssets();
			cross.Init(width, height, dataPath, asset_manager, this);
			cross.InitialCommercial(commercial);
			cross.Start();
		}
	}
	
	public void PromtToExit() {
		Log.d("Cross++", "Java PromtToExit");
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
			    new AlertDialog.Builder(CrossActivity.this)
		        .setIcon(android.R.drawable.ic_dialog_alert)
		        .setTitle("Confirm Exit")
		        .setMessage("Do you want to quit the game?")
		        .setPositiveButton("Yes", new DialogInterface.OnClickListener()   {
			        @Override
			        public void onClick(DialogInterface dialog, int which) {
			            finish();  
			        }
		        })
		        .setNegativeButton("No", null)
		        .show();
			}
		});
	}
	
	public void SendCommertialResult(int event) {
		if(cross != null) {
			cross.CommertialResult(event);
		} else {
			Log.d("Cross++", "Can't send CommertialResult. cross = null");
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