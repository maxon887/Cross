/*	Copyright � 2015 Lukyanau Maksim

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
#include "android/log.h"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, "Cross++", __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, "CrossException", __VA_ARGS__)

namespace cross{

class CommercialAndroid;

class LauncherAndroid : public Launcher{
public:
	LauncherAndroid(JNIEnv* env, jobject crossActivity, AAssetManager* assManager, string dataPath);
	~LauncherAndroid();

	int GetTargetWidth();
	int GetTargetHeight();
	string AssetsPath();
	string DataPath();
	void LogIt(const string& str);
    void LogIt(const char* formatStr, ...);
	File* LoadFile(const string& filename);
	void PromtToExit();
	void MessageBox(string message);
	void Exit();
	void Sleep(float milis);
	void InitializeCommercial(JNIEnv* env, jobject comm);
	Commercial* GetCommercial();
public:
	void SetTargetWidth(int width);
	void SetTargetHeight(int height);
	void DetachFromJVM();
	JNIEnv* GetJNIEnv();
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
