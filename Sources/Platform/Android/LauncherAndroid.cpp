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
#include "CommercialAndroid.h"
#include "File.h"

using namespace cross;

LauncherAndroid::LauncherAndroid(int w, int h, string dataPath, AAssetManager* assManager, jobject crossActivity, JNIEnv* env){
    LOGI("LauncherAndroid::LauncherAndroid");
	this->width = w;
	this->height = h;
	this->data_path = dataPath;
	this->asset_manager = assManager;
	this->cross_activity = crossActivity;
	this->commercial = NULL;
	env->GetJavaVM(&jvm);
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

void LauncherAndroid::LogIt(const char* formatStr, ...){
    va_list params;
    char buffer[1024];
    va_start(params, formatStr);
    vsprintf(buffer, formatStr, params);
    LOGI("%s", buffer);
    va_end(params);
}

File* LauncherAndroid::LoadFile(string filename) {
    AAsset* asset = AAssetManager_open(asset_manager, filename.c_str(), AASSET_MODE_STREAMING);
    if(!asset){
        throw CrossException("Can't load asset %s", filename.c_str());
    }
    File* file = new File();
    file->size = AAsset_getLength(asset);
    file->data = new CRByte[file->size];
    int read = AAsset_read(asset, file->data, file->size);
    if(read <= 0){
        throw CrossException("Can't load asset %s", filename.c_str());
    }
    AAsset_close(asset);
    return file;
}

void LauncherAndroid::PromtToExit(){
	JNIEnv* env = GetJNIEnv();
	jclass clazz = env->GetObjectClass(cross_activity);
	jmethodID methodID = env->GetMethodID(clazz, "PromtToExit", "()V");
	env->CallVoidMethod(cross_activity, methodID);;
}

void LauncherAndroid::InitializeCommercial(JNIEnv* env, jobject comm){
	LOGI("LauncherAndroid::InitializeCommercial");
	commercial = new CommercialAndroid(this, comm);
}

Commercial* LauncherAndroid::GetCommercial() {
	return commercial;
}

LauncherAndroid::~LauncherAndroid(){
	delete commercial;
}

JNIEnv* LauncherAndroid::GetJNIEnv(){
    JNIEnv* env;
    int getEnvStat = jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    switch(getEnvStat){
    case JNI_OK:
    	return env;
    case JNI_EDETACHED:
		LogIt("GetEnv: not attached");
		if (jvm->AttachCurrentThread(&env, NULL) != 0) {
			throw string("Failed to attach thread");
		}
		return env;
    case JNI_EVERSION:
    	throw string("GetEnv: version not supported");

    default:
    	throw string("Unknown JNI state");
    }
}


