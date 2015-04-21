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
#include "Game.h"
#include "Launcher.h"
#undef LoadString

class Saver{
//User module
public:

	Saver(Game* game);
	//Save string value for key
	//Important! string key and value can't contain space character
	void SaveString(const char* key, const char* value);
	//Save int value for key
	void SaveInt(const char* key, int value);
	//Save float value for key
	void SaveFloat(const char* key, float value);
	//Load string property from string key.
	//Important! returned string must to be deleted.
	//Return NULL if can't find key
	char* LoadString(const char* key);
	//Load int property from string key.
	//Return 0 if can't find key
	int LoadInt(const char* key);
	//Load float property from string key.
	//Return 0 if can't find key
	float LoadFloat(const char* key);
private:
//Framework module. You don't need call any of this methods or modify variable
	//Launcher* launcher;
	char prefs_path[BUF_LEN];
	char copy_path[BUF_LEN];
	char str_buf[BUF_LEN];
};
