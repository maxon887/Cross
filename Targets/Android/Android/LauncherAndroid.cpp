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

Sound* LauncherAndroid::CreateSound(const char* filename, bool loop){
	unsigned char* data;
	int length;
	LoadFile(filename, &data, &length);
	return new AudioAndroid(data, length, loop);
}

Music* LauncherAndroid::CreateMusic(const char* filename, bool loop){
	//unsigned char* data;
	//int length;
	//LoadFile(filename, &data, &length);
	//return new AudioAndroid(data, length, loop);
	off_t start;
	off_t length;
	int desc = LoadDescriptor(filename, &start, &length);
	return new MusicAndroid(desc, start, length);
}

void LauncherAndroid::LoadFile(const char* filename, unsigned char** buffer, int* length){
	AAsset* asset = AAssetManager_open(asset_manager, filename, AASSET_MODE_STREAMING);
	if(!asset){
		sprintf(str_buff, "Can not load asset %s", filename);
		throw str_buff;
	}
	//long fileSize = AAsset_getLength(asset);
	*length = AAsset_getLength(asset);
	//unsigned char* fileData = (unsigned char*)malloc(fileSize);
	*buffer = (unsigned char*)malloc(*length);
	int read = AAsset_read(asset, *buffer, *length);
	AAsset_close(asset);
	if(read != *length){
		sprintf(str_buff, "File %s was particaly read", filename);
		throw str_buff;
	}
}

int LauncherAndroid::LoadDescriptor(const char* filename, off_t* start, off_t* length){
	AAsset* asset = AAssetManager_open(asset_manager, filename, AASSET_MODE_STREAMING);
	int desc = AAsset_openFileDescriptor(asset, start, length);
	AAsset_close(asset);
	return desc;
}
/*
unsigned char* LauncherAndroid::ImageFromAssets(const char* filename, int* width, int* height){
	AAsset* asset = AAssetManager_open(asset_manager, filename, AASSET_MODE_STREAMING);
	if(!asset){
		sprintf(str_buff, "Can not load asset %s", filename);
		throw str_buff;
	}
	long fileSize = AAsset_getLength(asset);
	unsigned char* file = (unsigned char*)malloc(fileSize);
	int read = AAsset_read(asset, file, fileSize);
	AAsset_close(asset);
	unsigned char* image = SOIL_load_image_from_memory(file, fileSize, width, height, 0, SOIL_LOAD_RGBA);
	free(file);
	return image;
}*/






