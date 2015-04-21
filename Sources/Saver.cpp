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
	
#include "Saver.h"

Saver::Saver(Game* game){
	sprintf(prefs_path, "%s/prefs", game->launcher->DataPath());
	sprintf(copy_path, "%s/prefs.copy", game->launcher->DataPath());
}

void Saver::SaveString(const char* saveKey, const char* saveValue){
	FILE* prefs = fopen(prefs_path, "r");
	FILE* copy = fopen(copy_path, "w");
	if(prefs == NULL || copy == NULL){
		throw "Preferens file access exception";
	}
	char line[256];
	bool saved = false;
	while (fgets(line, 256, prefs) != NULL)	{
		char* key = strtok(line, " ");
		char* value = strtok(NULL, " ");
		if(strcmp(key, saveKey) != 0){
			sprintf(str_buf, "%s %s", key, value);
			fwrite(str_buf, strlen(str_buf), 1, copy);
		}else{
			sprintf(str_buf, "%s %s\n", saveKey, saveValue);
			fwrite(str_buf, strlen(str_buf), 1, copy);
			saved = true;
		}
	}
	if(!saved){
		sprintf(str_buf, "%s %s\n", saveKey, saveValue);
		fwrite(str_buf, strlen(str_buf), 1, copy);
	}
	fclose(prefs);
	fclose(copy);
	remove(prefs_path);
	rename(copy_path, prefs_path);
}

void Saver::SaveInt(const char* key, int value){
	char buf[128];
	sprintf(buf, "%d", value);
	SaveString(key, buf);
}

void Saver::SaveFloat(const char* key, float value){
	char buf[128];
	sprintf(buf, "%f", value);
	SaveString(key, buf);
}

char* Saver::LoadString(const char* loadKey){
 	FILE* prefs = fopen(prefs_path, "r");
	if(prefs == NULL){
		prefs = fopen(prefs_path, "w");
		if(prefs == NULL){
			sprintf(str_buf, "Can't create property file at path: %s", prefs_path);
			throw str_buf;
		}
		fclose(prefs);
		return NULL;
	}
	char line[256];
	while(fgets(line, 256, prefs) != NULL){
		char* key = strtok(line, " ");
		char* value = strtok(NULL, " ");
		if(strcmp(key, loadKey) == 0){
			char* ret = new char[strlen(value) + 1];
			strcpy(ret, value);
			fclose(prefs);
			return ret;
		}
	}
	fclose(prefs);
	return NULL;
}

int Saver::LoadInt(const char* key){
	char* strValue = LoadString(key);
	if(strValue == NULL)
		return 0;
	int ret = atoi(strValue);
	delete[] strValue;
	return ret;
}

float Saver::LoadFloat(const char* key){
	const char* strValue = LoadString(key);
	if(strValue == NULL)
		return 0;
	float ret = (float)atof(strValue);
	delete[] strValue;
	return ret;
}
