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
	const char* appname = env->GetStringUTFChars(packageName, NULL);
	app->activity->vm->DetachCurrentThread();
	sprintf(data_path, "/data/data/%s/files", appname);
}

int LauncherAndroid::GetTargetWidth(){
	return width;
}

int LauncherAndroid::GetTargetHeight(){
	return height;
}

const char* LauncherAndroid::DataPath(){
	return data_path;
}

void LauncherAndroid::LogIt(const char* str){
	__android_log_print(ANDROID_LOG_DEBUG, "Cross++", "%s", str);
}

void LauncherAndroid::FileFromAssets(const char* file){
	//LogIt("Try load file from assets");
	AAsset* asset = AAssetManager_open(asset_manager, file, AASSET_MODE_STREAMING);
	if(!asset){
		sprintf(str_buff, "Can not load asset %s", file);
		throw str_buff;
	}
	sprintf(str_buff, "%s/%s", DataPath(), file);
	char buf[1024];
 	int nb_read = 0;
	FILE* out = fopen(str_buff, "w");
	if(!out){
		sprintf(str_buff, "Can not load asset %s", str_buff);
		throw str_buff;
	}
	while((nb_read = AAsset_read(asset, buf, 1024)) > 0)
		fwrite(buf, nb_read, 1, out);
	fclose(out);
	AAsset_close(asset);
}























