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
#include "SOIL.h"
#include "Utils/Debuger.h"

LauncherAndroid::LauncherAndroid(android_app* app, int width, int height){
	this->app = app;
	this->asset_manager = app->activity->assetManager;
	this->width = width;
	this->height = height;
	JNIEnv *env;
	app->activity->vm->AttachCurrentThread(&env, NULL);
	jclass android_content_Context = env->GetObjectClass(app->activity->clazz);
	jmethodID midGetPackageName = env->GetMethodID(android_content_Context, "getPackageName", "()Ljava/lang/String;");
	jstring packageName = (jstring)env->CallObjectMethod(app->activity->clazz, midGetPackageName);
	string appname = env->GetStringUTFChars(packageName, NULL);
	app->activity->vm->DetachCurrentThread();
	data_path = "/data/data/" + appname + "/files";
	//sprintf(data_path, "/data/data/%s/files", appname);
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
	__android_log_print(ANDROID_LOG_DEBUG, "Cross++", "%s", str.c_str());
}

Sound* LauncherAndroid::CreateSound(string filename, bool loop){
	return new AudioAndroid(filename, loop, false);
}

Music* LauncherAndroid::CreateMusic(string filename, bool loop){
	return new AudioAndroid(filename, loop, true);
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
	AAsset_close(asset);
}





