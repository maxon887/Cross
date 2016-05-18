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
#pragma once
#include "Launcher.h"
#include "Input.h"

#include <Windows.h>

namespace cross{

class LauncherWIN : public Launcher{
public:
	LauncherWIN(HWND wnd);
	~LauncherWIN();
	int GetTargetWidth();
	int GetTargetHeight();
	string AssetsPath();
	string DataPath();
	File* LoadFile(const string& filename);
	void LogIt(const string& msg);
	void LogIt(const char* formatString, ...);
	void Sleep(float milis);
	void ShowMessage(const string& msg);
	void ResizeWindow(int posX, int posY, int width, int height);
public:
	void SetWindowPosition(int x, int y);
	void SetWindowSize(int width, int heght);

	void KeyReleasedHandle(Key key);
private:
	HWND wnd;
	char* assets_path;
	int window_pos_x;
	int window_pos_y;
	int window_width;
	int window_height;
};

}