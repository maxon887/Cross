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

package com.cross.android;

import com.cross.MainActivity;

import android.view.Display;

public class Launcher {
	
	private int width;
	private int height;
	
	@SuppressWarnings("deprecation")
	public Launcher() {
		Display display = MainActivity.self.getWindowManager().getDefaultDisplay();
		width = display.getWidth();
		height = display.getHeight();
	}
	
	public int GetTargetWidth() {
		return width;
	}
	public int GetTargetHeight() {
		return height;
	}
	/*
	public void LogIt(String str) {
		Log.d("Cross++", str);
	}*/
}
