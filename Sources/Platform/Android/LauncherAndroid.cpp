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

#include "LauncherAndroid.h"
#include "CommercialAndroid.h"
#include "Cross.h"
#include <string>

using namespace cross;
using namespace std;

LauncherAndroid::LauncherAndroid(int w, int h, string dataPath, AAssetManager* assManager, jobject crossActivity, JNIEnv* env){
	this->width = w;
	this->height = h;
	this->data_path = dataPath;
	this->asset_manager = assManager;
	this->cross_activity = crossActivity;
	this->env = env;
	this->commercial = NULL;
}

int LauncherAndroid::GetTargetWidth(){
	return width;
}

int LauncherAndroid::GetTargetHeight(){
	return height;
}

string LauncherAndroid::AssetsPath(){
	return "This platform do not specify application assets folder.\nAll assets needs to be load through asset manager";
}

string LauncherAndroid::DataPath(){
	return data_path;
}

void LauncherAndroid::LogIt(string str){
	LOGI("%s", str.c_str());
}

void LauncherAndroid::LoadFile(string filename, unsigned char** buffer, int* length){
	AAsset* asset = AAssetManager_open(asset_manager, filename.c_str(), AASSET_MODE_STREAMING);
	if(!asset){
		string msg = "Can't load asset " + filename;
		throw msg;
	}
	*length = AAsset_getLength(asset);
	*buffer = (unsigned char*)malloc(*length);
	int read = AAsset_read(asset, *buffer, *length);
	if(read <= 0){
		string msg = "Can't load asset " + filename;
		throw msg;
	}
	AAsset_close(asset);
}

void LauncherAndroid::PromtToExit(){
	jclass clazz = env->GetObjectClass(cross_activity);
	jmethodID methodID = env->GetMethodID(clazz, "PromtToExit", "()V");
	env->CallVoidMethod(cross_activity, methodID);
}

void LauncherAndroid::InitializeCommercial(JNIEnv* env, jobject comm){
	LOGI("LauncherAndroid::InitializeCommercial");
	commercial = new CommercialAndroid(env, comm);
}

Commercial* LauncherAndroid::GetCommercial() {
	return commercial;
}

LauncherAndroid::~LauncherAndroid(){
	delete commercial;
}



