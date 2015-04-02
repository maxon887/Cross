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

LauncherAndroid::LauncherAndroid(AAssetManager* asset_manager, int width, int height){
	this->asset_manager = asset_manager;
	this->width = width;
	this->height = height;
}

int LauncherAndroid::GetTargetWidth(){
	return width;
}

int LauncherAndroid::GetTargetHeight(){
	return height;
}

const char* LauncherAndroid::DataPath(){
	return "/data/data/com.cross/files";
}

void LauncherAndroid::LogIt(const char* str){
	__android_log_print(ANDROID_LOG_DEBUG, "Cross++", "%s", str);
}

void LauncherAndroid::FileFromAssets(const char* file){
	LogIt("Try load file from assets");
	AAsset* asset = AAssetManager_open(asset_manager, file, AASSET_MODE_STREAMING);
	sprintf(str_buff, "%s%s", DataPath(), file);
	char buf[1024];
	int nb_read = 0;
	FILE* out = fopen(str_buff, "rw");
	while((nb_read = AAsset_read(asset, buf, 1024)) > 0)
			fwrite(buf, nb_read, 1, out);
	fclose(out);
	AAsset_close(asset);
	/*
	AAssetDir* assetDir = AAssetManager_openDir(asset_manager, "");
	const char* filename = (const char*)NULL;
	while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) {
	    AAsset* asset = AAssetManager_open(asset_manager, filename, AASSET_MODE_STREAMING);
	    char buf[256];
	    int nb_read = 0;
	    FILE* out = fopen(filename, "rw");
	    while ((nb_read = AAsset_read(asset, buf, 256)) > 0)
	        fwrite(buf, nb_read, 1, out);
	    fclose(out);
	    AAsset_close(asset);
	}
	AAssetDir_close(assetDir);*/
}
