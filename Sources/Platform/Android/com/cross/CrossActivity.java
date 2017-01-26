package com.cross;

import org.fmod.FMOD;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.view.Window;
import android.view.View;

public class CrossActivity extends Activity implements SurfaceHolder.Callback{
	private static final String TAG 	= "CrossJava";
	private Cross cross 				= null;
	private AssetManager asset_manager 	= null;
	private Commercial commercial		= null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.d(TAG, "onCreate");
		super.onCreate(savedInstanceState);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        SurfaceView surfaceView = new SurfaceView(this);
        surfaceView.setEnabled(true);
        surfaceView.setFocusable(true);
        surfaceView.setFocusableInTouchMode(true);
        surfaceView.getHolder().addCallback(this);
		setContentView(surfaceView);
		System.loadLibrary("fmod");
		System.loadLibrary("freetype2");
        System.loadLibrary("assimp");
		System.loadLibrary("cross");
		cross = new Cross();

		FMOD.init(this);
		String dataPath = getFilesDir().getPath();
		asset_manager = getResources().getAssets();

		cross.OnCreate(this, asset_manager, dataPath);

		try{
			//commercial = new Commercial(this);
		}catch(Throwable ex){
			Log.d(TAG, "can not create commercial");
			commercial = null;
		}
		//rate it
	    RateThisApp.onStart(this);
	    RateThisApp.showRateDialogIfNeeded(this);
	}

	@Override
	protected void onStart() {
		Log.d(TAG, "onStart");
		super.onStart();
	}

	@Override
	protected void onResume() {
		Log.d(TAG, "onResume");
		super.onResume();
		cross.OnResume();
	}

	@Override
	protected void onPause() {
		Log.d(TAG, "onPause");
		super.onPause();
		cross.OnSuspend();
	}

	@Override
	protected void onStop() {
		Log.d(TAG, "onStop");
		super.onStop();
	}

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            if (Build.VERSION.SDK_INT >= 11) {
                View decorView = getWindow().getDecorView();
                decorView.setSystemUiVisibility(  View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                                | View.SYSTEM_UI_FLAG_FULLSCREEN
                                                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
            }
        }
    }

	@Override
	public void surfaceCreated(SurfaceHolder surfaceHolder) {

	}

	@Override
	public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int w, int h) {
		cross.SurfaceChanged(surfaceHolder.getSurface(), w, h);
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
		cross.SurfaceDestroyed();
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int actionIndex = event.getActionIndex();
		int actionId = event.getPointerId(actionIndex);
		int actionMasked = event.getActionMasked();
		switch (actionMasked){
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:{
				cross.ActionDown(event.getX(actionIndex), event.getY(actionIndex), actionId);
				return true;
			}
			case MotionEvent.ACTION_MOVE:{
				for(int i = 0; i < event.getPointerCount(); i++){
					cross.ActionMove(event.getX(i), event.getY(i), event.getPointerId(i));
				}
				return true;
			}
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP: {
				cross.ActionUp(event.getX(actionIndex), event.getY(actionIndex), actionId);
				return true;
			}
		}
		return super.onTouchEvent(event);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		switch (keyCode) {
			case KeyEvent.KEYCODE_BACK:
				cross.PressKey(Cross.KEY_BACK);
				return true;
            default:
                cross.PressKey(keyCode);
                return true;
		}
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		switch(keyCode){
		case KeyEvent.KEYCODE_BACK:
			cross.ReleaseKey(Cross.KEY_BACK);
			return true;
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

	public void MessageBox(String msg){
		final String thrMsg = msg;
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				AlertDialog.Builder builder  = new AlertDialog.Builder(CrossActivity.this);
				builder.setIcon(android.R.drawable.ic_dialog_alert);
				builder.setMessage(thrMsg);
				builder.setTitle("Unhandled exception");
				builder.setCancelable(false);
				builder.setPositiveButton("Ok",
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int which) {
								FMOD.close();
								System.exit(0);
							}
						});
				builder.create().show();
			}
		});
	}

	public void PromtToExit() {
		Log.d(TAG, "PromtToExit");

		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				AlertDialog.Builder builder = new AlertDialog.Builder(CrossActivity.this);
				builder.setIcon(android.R.drawable.ic_dialog_alert);
				builder.setTitle("Confirm Exit");
				builder.setMessage("Do you want to quit the game?");
				builder.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						cross.OnExit();
					}
				});
				builder.setNegativeButton("No", null);
				builder.show();
			}
		});
	}

	public void Exit(){
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				FMOD.close();
				System.exit(0);
			}
		});
	}

	public void SendCommertialResult(int event) {
		if(cross != null) {
			cross.CommertialResult(event);
		} else {
			Log.d(TAG, "Can't send CommertialResult. cross = null");
		}
	}
}