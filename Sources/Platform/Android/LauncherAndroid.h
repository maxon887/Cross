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
#include <jni.h>
#include <android/asset_manager.h>

namespace cross{

class CommercialAndroid;

class LauncherAndroid : public Launcher{
public:
	LauncherAndroid(int w, int h, string dataPath, AAssetManager* assManager, jobject crossActivity, JNIEnv* env);
	int GetTargetWidth();
	int GetTargetHeight();
	string AssetsPath();
	string DataPath();
	void LogIt(string str);
	void LoadFile(string filename, unsigned char** buffer, int* length);
	void PromtToExit();
	void InitializeCommercial(JNIEnv* env, jobject comm);
	Commercial* GetCommercial();
	JNIEnv* GetJNIEnv();
	~LauncherAndroid();
private:
	JavaVM* jvm;
	jobject cross_activity;
	AAssetManager* asset_manager;
	CommercialAndroid* commercial;
	string data_path;
	int width;
	int height;
};

}
