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

#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>
#include "Launcher.h"
#include "AudioAndroid.h"
#include "MusicAndroid.h"

class LauncherAndroid : public Launcher{
public:
	LauncherAndroid(android_app* app, int width, int height);
	int GetTargetWidth();
	int GetTargetHeight();
	const char* DataPath();
	void LogIt(const char* str);
	Sound* CreateSound(const char* filename, bool loop);
	Music* CreateMusic(const char* filename, bool loop);
	void LoadFile(const char* filename, unsigned char** buffer, int* length);
	int LoadDescriptor(const char* filename, off_t* start, off_t* length);
	//unsigned char* ImageFromAssets(const char* filename, int* width, int* height);
private:
	int width;
	int height;
	char data_path[BUF_LEN];
	char str_buff[BUF_LEN];
	AAssetManager* asset_manager;
	android_app* app;
};
