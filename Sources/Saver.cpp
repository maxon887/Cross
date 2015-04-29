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
	prefs_path = game->launcher->DataPath() + "/prefs";
	copy_path = game->launcher->DataPath() + "/prefs.copy";
	game->launcher->LogIt("Saver initialized");
}

void Saver::SaveString(string key, string value){
	ifstream prefs;
	prefs.open(prefs_path);
	ofstream prefs_copy;
	prefs_copy.open(copy_path);
	string locKey;
	string locVal;
	bool saved = false;
	while(prefs >> locKey){
		prefs >> locVal;
		if(locKey != key){
			prefs_copy << locKey << " " << locVal << endl;
		}else{
			prefs_copy << locKey << " " << value << endl;
			saved = true;
		}
	}
	if(!saved){
		prefs_copy << key << " " << value << endl;
	}
	prefs.close();
	prefs_copy.close();
	remove(prefs_path.c_str());
	rename(copy_path.c_str(), prefs_path.c_str());
}

void Saver::SaveInt(string key, int value){
	char buf[128];
	sprintf(buf, "%d", value);
	SaveString(key, buf);
}

void Saver::SaveFloat(string key, float value){
	char buf[128];
	sprintf(buf, "%f", value);
	SaveString(key, buf);
}

void Saver::SaveBool(string key, bool value){
	SaveInt(key, value);
}

string Saver::LoadString(string key){
	ifstream prefs;
	prefs.open(prefs_path);
	string locKey;
	string value;
	while(prefs >> locKey){
		prefs >> value;
		if(locKey == key){
			prefs.close();
			return value;
		}
	}
	prefs.close();
	return value;
}

int Saver::LoadInt(string key){
	string strValue = LoadString(key);
	if(strValue.empty())
		return 0;
	int ret = atoi(strValue.c_str());
	return ret;
}

float Saver::LoadFloat(string key){
	string strValue = LoadString(key);
	if(strValue.empty())
		return 0;
	float ret = (float)atof(strValue.c_str());
	return ret;
}

bool Saver::LoadBool(string key){
	return LoadInt(key);
}
