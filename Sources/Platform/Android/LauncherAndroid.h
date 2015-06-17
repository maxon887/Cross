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

//#include "Game.h"
#include "Launcher.h"
#include <android/log.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>

using namespace cross;

class LauncherAndroid : public Launcher{
public:
	LauncherAndroid(android_app* app, int width, int height);
	int GetTargetWidth();
	int GetTargetHeight();
	string AssetsPath();
	string DataPath();
	void LogIt(string str);
	void LoadFile(string filename, unsigned char** buffer, int* length);
	~LauncherAndroid();
private:
	int width;
	int height;
	string data_path;
	AAssetManager* asset_manager;
	android_app* app;
};
