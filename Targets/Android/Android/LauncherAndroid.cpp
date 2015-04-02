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

#include <jni.h>

LauncherAndroid::LauncherAndroid(AAssetManager* asset_manager, const char* packageName, int width, int height){
	this->asset_manager = asset_manager;
	this->width = width;
	this->height = height;
	//memset(data_path, 0, BUF_LEN);
	sprintf(data_path, "/data/data/%s/files", packageName);
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
	LogIt("Try load file from assets");
	AAsset* asset = AAssetManager_open(asset_manager, file, AASSET_MODE_STREAMING);
	//memset(str_buff, 0, BUF_LEN);
	sprintf(str_buff, "%s/%s", DataPath(), file);
	char buf[1024];
 	int nb_read = 0;
	FILE* out = fopen(str_buff, "w");
	while((nb_read = AAsset_read(asset, buf, 1024)) > 0)
			fwrite(buf, nb_read, 1, out);
	fclose(out);
	AAsset_close(asset);
}
