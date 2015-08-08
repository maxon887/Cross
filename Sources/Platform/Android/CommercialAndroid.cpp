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
	
#include "CommercialAndroid.h"
#include "Cross.h"

CommercialAndroid::CommercialAndroid(JNIEnv* env, jobject comm){
	LOGI("CommercialAndroid::CommercialAndroid");
	if(env == NULL || comm == 0)
		throw string("JNIEnv not initialized");
	this->env = env;
	this->comm = comm;
}

void CommercialAndroid::DownloadAd(){
	LOGI("CommercialAndroid::DownloadAd");
	jclass clazz = env->GetObjectClass(comm);
	jmethodID methodID = env->GetMethodID(clazz, "DownloadAd", "()V");
	env->CallVoidMethod(comm, methodID);
}

void CommercialAndroid::ShowAd(){
	LOGI("CommercialAndroid::ShowAd");
	jclass clazz = env->GetObjectClass(comm);
	jmethodID methodID = env->GetMethodID(clazz, "ShowAd", "()V");
	env->CallVoidMethod(comm, methodID);
}

void CommercialAndroid::Purchase(){
	LOGI("CommercialAndroid::Purchase");
	jclass clazz = env->GetObjectClass(comm);
	jmethodID methodID = env->GetMethodID(clazz, "Purchase", "()V");
	env->CallVoidMethod(comm, methodID);
}
