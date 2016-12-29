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
	~Config();
	/* Custom user properties */
	void SetString(const string& key, const string& value);
	void SetInt(const string& key, S32 value);
	void SetFloat(const string& key, float value);
	void SetBool(const string& key, bool value);
	string GetString(const string& key, const string& def);
	S32 GetInt(const string& key, S32 def);
	float GetFloat(const string& key, float def);
	bool GetBool(const string& key, bool def);
	/* Game config properties */
	bool UseCompressedTextures();
	Texture::Filter GetTextureFilter();
	float GetViewDistance();
	bool IsOffscreenRender();

protected:
	string user_config_path;
	Dictionary<string, string> user_prefs;

	bool use_compressed_textures;
	Texture::Filter texture_filter;
	float view_distance;
	bool offscreen_render;

	string GetString(const string& key);

	void LoadGameConfig(File* file);
	void LoadUserConfig();
	void SaveUserConfig();
};
    
}
