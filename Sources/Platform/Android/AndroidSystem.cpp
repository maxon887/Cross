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
#include "AndroidSystem.h"
#include "File.h"
#include <unistd.h>

using namespace cross;

AndroidSystem::AndroidSystem(JNIEnv* env, jobject crossActivity, AAssetManager* assManager, string dataPath){
    LOGI("AndroidSystem::AndroidSystem");
	this->data_path = dataPath;
	this->asset_manager = assManager;
	this->cross_activity = crossActivity;
	env->GetJavaVM(&jvm);
}

AndroidSystem::~AndroidSystem(){
    LOGI("AndroidSystem::~AndroidSystem");
}

string AndroidSystem::AssetsPath(){
	return "This platform do not specify application assets folder.All assets needs to be load through asset manager";
}

string AndroidSystem::DataPath(){
	return data_path;
}

void AndroidSystem::Log(const char* str){
	LOGI("%s", str);
}

File* AndroidSystem::LoadAssetFile(const string& filename) {
    AAsset* asset = AAssetManager_open(asset_manager, filename.c_str(), AASSET_MODE_STREAMING);
    if(!asset){
		throw CrossException("Can't load asset %s", filename.c_str());
    }
    File* file = new File();
    file->size = AAsset_getLength(asset);
    file->data = new Byte[file->size];
    int read = AAsset_read(asset, file->data, file->size);
    if(read <= 0){
        throw CrossException("Can't load asset %s", filename.c_str());
    }
    AAsset_close(asset);
    return file;
}

U64 AndroidSystem::GetTime(){
	struct timeval ptv;
	gettimeofday(&ptv, NULL);
	return (ptv.tv_usec + ptv.tv_sec * 1000000LL);
}

void AndroidSystem::PromtToExit(){
	JNIEnv* env = GetJNIEnv();
	jclass clazz = env->GetObjectClass(cross_activity);
	jmethodID methodID = env->GetMethodID(clazz, "PromtToExit", "()V");
	env->CallVoidMethod(cross_activity, methodID);
}

void AndroidSystem::RequestOrientation(Orientation orientation) {
    JNIEnv* env = GetJNIEnv();
    jclass clazz = env->GetObjectClass(cross_activity);
    jmethodID methodID = env->GetMethodID(clazz, "RequestOrientation", "(I)V");
    env->CallVoidMethod(cross_activity, methodID, orientation);
}

void AndroidSystem::MessageBox(string message) {
    JNIEnv* env = GetJNIEnv();
    jclass clazz = env->GetObjectClass(cross_activity);
    jmethodID methodID = env->GetMethodID(clazz, "MessageBox", "(Ljava/lang/String;)V");
    env->CallVoidMethod(cross_activity, methodID, env->NewStringUTF(message.c_str()));
}

void AndroidSystem::Exit() {
    JNIEnv* env = GetJNIEnv();
    jclass clazz = env->GetObjectClass(cross_activity);
    jmethodID methodID = env->GetMethodID(clazz, "Exit", "()V");
    env->CallVoidMethod(cross_activity, methodID);
    jvm->DetachCurrentThread();
}

void AndroidSystem::Sleep(float milis) {
    usleep(milis*1000);
}

void AndroidSystem::CallActivityVoidMethod(const string &methodName) {
    JNIEnv* env = GetJNIEnv();
    jclass clazz = env->GetObjectClass(cross_activity);
    jmethodID methodID = env->GetMethodID(clazz, methodName.c_str(), "()V");
    env->CallVoidMethod(cross_activity, methodID);
}

void AndroidSystem::CallActivityVoidMethod(const string& methodName, const string& parameter){
    JNIEnv* env = GetJNIEnv();
    jclass clazz = env->GetObjectClass(cross_activity);
    jmethodID methodID = env->GetMethodID(clazz, methodName.c_str(), "(Ljava/lang/String;)V");
    env->CallVoidMethod(cross_activity, methodID, env->NewStringUTF(parameter.c_str()));
}

void AndroidSystem::DetachFromJVM() {
    jvm->DetachCurrentThread();
}

JNIEnv* AndroidSystem::GetJNIEnv(){
    JNIEnv* env;
    int getEnvStat = jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    switch(getEnvStat){
    case JNI_OK:
    	return env;
    case JNI_EDETACHED:
		LogIt("Attempt to attach thread to JVM");
		if (jvm->AttachCurrentThread(&env, NULL) != 0) {
			throw CrossException("Failed to attach thread");
		}
		return env;
    case JNI_EVERSION:
    	throw CrossException("Java environment: version not supported");

    default:
    	throw CrossException("Unknown JNI state");
    }
}


