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

#include <Windows.h>

#define TARGET_WIDTH 512		
#define TARGET_HEIGHT 512
// 16:9
//#define TARGET_WIDTH 540		
//#define TARGET_HEIGHT 960
//iPhone 5
//#define TARGET_WIDTH 320
//#define TARGET_HEIGHT 568	
// 4:3
//#define TARGET_WIDTH 320 
//#define TARGET_HEIGHT 480

namespace cross{

class LauncherWIN : public Launcher{
public:
	LauncherWIN(HWND wnd);
	~LauncherWIN();
	void SetTargetSize(int width, int height);
	int GetTargetWidth();
	int GetTargetHeight();
	string AssetsPath();
	string DataPath();
	unsigned char* LoadFile(string filename, int *size);
	File* LoadFile(string filename);
	void LogIt(string msg);
	void LogIt(const char* formatString, ...);
	void Sleep(float milis);
	void ShowMessage(string msg);
	void LandscapeMode(bool land);
private:
	HWND wnd;
	char* assets_path;
	bool landscape;
	int target_width;
	int target_height;
};

}