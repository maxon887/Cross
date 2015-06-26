package com.cross;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class CrossRenderer extends GLSurfaceView
						implements GLSurfaceView.Renderer{

	private CrossActivity activity;

	public CrossRenderer(Context context) {
		super(context);
		this.activity = (CrossActivity)context;
		setRenderer(this);
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		activity.Update();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		activity.Init(width, height);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		activity.ReleaseCross();
	}

}
