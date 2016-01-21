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
#include "Config.h"

#include <fstream>

using namespace cross;

Config::Config(string dataPath){
	prefs_path = dataPath + "/prefs";
	copy_path = dataPath + "/prefs.tmp";
}

void Config::SaveString(string key, string value){
	if(key.find(" ") != string::npos || value.find(" ") != string::npos){
		throw CrossException("Key and value can't contains space characters");
	}
	ifstream prefs;
	prefs.open(prefs_path);
	ofstream prefs_copy;
	prefs_copy.open(copy_path);
	string locKey;
	string locVal;
	bool saved = false;
	while(prefs >> locKey) {
		prefs >> locVal;
		if(locKey != key) {
			prefs_copy << locKey << " " << locVal << endl;
		} else {
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

void Config::SaveInt(string key, int value){
	SaveString(key, to_string(value));
}

void Config::SaveFloat(string key, float value){
	SaveString(key, to_string(value));
}

void Config::SaveBool(string key, bool value){
	SaveInt(key, value);
}

string Config::LoadString(string key, string def){
	string strValue = LoadString(key);
	if(strValue.empty())
		return def;
	return strValue;
}

int Config::LoadInt(string key, int def){
	string strValue = LoadString(key);
	if(strValue.empty())
		return def;
	int ret = atoi(strValue.c_str());
	return ret;
}

float Config::LoadFloat(string key, float def){
	string strValue = LoadString(key);
	if(strValue.empty())
		return def;
	float ret = (float)atof(strValue.c_str());
	return ret;
}

bool Config::LoadBool(string key, bool def){
	return LoadInt(key, def) != 0;
}

string Config::LoadString(string key){
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
	return string("");
}
