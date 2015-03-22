/*	Copyright © 2015 Lukyanau Maksim

	This file is part of Cross++ Game Engine.

    Cross++ Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cross++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/

package com.cross;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.cross.android.IMP;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class GLRenderView extends GLSurfaceView
implements GLSurfaceView.Renderer {
	
	private IMP imp;
	
	public GLRenderView(Context context) {
		super(context);
		setRenderer(this);
		imp = new IMP();
	}

	@Override
	public void onDrawFrame(GL10 gl) { 
		imp.Update();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) { }

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		try {
			Thread.sleep(0);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		imp.Start();
	}

}
