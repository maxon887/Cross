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
#include "System.h"
#include "Input.h"

#include <Windows.h>

namespace cross{

class WINSystem : public System{
public:
	WINSystem(HWND wnd = 0);
	~WINSystem();

	string AssetsPath();
	string DataPath();
	U64 GetTime();
	void Log(const char* msg);
	void Sleep(float milis);
	void ShowMessage(const string& msg);
	void FullScreen(bool yes);
	void ResizeWindow(int posX, int posY, int width, int height);

public:
	void SetWND(HWND wnd);
	void SetWindowPosition(int x, int y);

	void KeyReleasedHandle(Key key);
	
private:
	HWND wnd;
	char* assets_path;
	int window_pos_x;
	int window_pos_y;
	bool fullscreen;

	bool EnterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate);
	bool ExitFullscreen(HWND hwnd, int windowX, int windowY, int windowedWidth, int windowedHeight, int windowedPaddingX, int windowedPaddingY);
};

}