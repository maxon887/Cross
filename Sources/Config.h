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
#include "Cross.h"
#include "Texture.h"

namespace cross {

/*	Configuration manager. This class provide interface 
	for saving and loading data in non-volatile device memory. */
class Config{
public:
	Config();
	//Save string value for key.
	//Important! string key and value can't contain space characters
	void SaveString(string key, string value);
	//Save int value for key
	void SaveInt(string key, S32 value);
	//Save float value for key
	void SaveFloat(string key, float value);
	//Save boolean value for key
	void SaveBool(string key, bool value);
	//Load string property from string key.
	string LoadString(string key, string def);
	//Load int property from string key.
	S32 LoadInt(string key, S32 def);
	//Load float property from string key.
	float LoadFloat(string key, float def);
	//Load boolean property from string key.
	bool LoadBool(string key, bool def);

	//default config properties
	Texture::Filter GetTextureFilter();

protected:
	string prefs_path;
	string copy_path;

	Texture::Filter texture_filter;

	string LoadString(string key);

	void InitializeDefaultConfig(File* file);
};
    
}
