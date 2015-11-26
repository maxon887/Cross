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
#pragma once

#include <string>

using namespace std;

namespace cross {

/* Configuration manager. Cross platform way for save and load game data(score, config, etc)*/
class Config{
public:
	//Save string value for key.
	//Important! string key and value can't contain space character
	void SaveString(string key, string value);
	//Save int value for key
	void SaveInt(string key, int value);
	//Save float value for key
	void SaveFloat(string key, float value);
	//Save boolean value for key
	void SaveBool(string key, bool value);
	//Load string property from string key.
	//Important! returned string must to be deleted.
	//Return NULL if can't find key
	string LoadString(string key, string def);
	//Load int property from string key.
	//Return 0 if can't find key
	int LoadInt(string key, int def);
	//Load float property from string key.
	//Return 0 if can't find key
	float LoadFloat(string key, float def);
	//Load boolean property from string key.
	bool LoadBool(string key, bool def);
//Internal data. You don't need call any of this methods or modify variable
public:
	Config(string dataPath);
private:
	string prefs_path;
	string copy_path;
	string LoadString(string key);
};
    
}
