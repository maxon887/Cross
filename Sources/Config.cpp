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
#include "Launcher.h"
#include "File.h"

#include <fstream>

#include "Libs/TinyXML/tinyxml.h"

using namespace cross;

Config::Config(){
	string dataPath = launcher->DataPath();
	prefs_path = dataPath + "/prefs";
	copy_path = dataPath + "/prefs.tmp";
	File* config = launcher->LoadFile("Config.xml");
	InitializeDefaultConfig(config);
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

void Config::SaveInt(string key, S32 value){
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

S32 Config::LoadInt(string key, S32 def){
	string strValue = LoadString(key);
	if(strValue.empty())
		return def;
	return atoi(strValue.c_str());
}

float Config::LoadFloat(string key, float def){
	string strValue = LoadString(key);
	if(strValue.empty())
		return def;
	return (float)atof(strValue.c_str());
}

bool Config::LoadBool(string key, bool def){
	return LoadInt(key, def) != 0;
}

Texture::Filter Config::GetTextureFilter(){
	return texture_filter;
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

void Config::InitializeDefaultConfig(File* xmlFile){
	TiXmlDocument xml;
	Byte* source = new Byte[xmlFile->size + 1]; // +1 for null terminated string
	memcpy(source, xmlFile->data, xmlFile->size);
	source[xmlFile->size] = 0;
	delete xmlFile;
	xml.Parse((const char*)source, 0, TIXML_ENCODING_UTF8);
	delete source;

	TiXmlHandle xmlDoc(&xml);
	TiXmlElement* root;
	TiXmlElement* element;

	root = xmlDoc.FirstChildElement("Config").Element();
	if(root){
		element = root->FirstChildElement("property");
		while(element){
			string name = element->Attribute("name");

			if(name == "TextureFilter"){
				string strValue = element->Attribute("value");
				if(strValue == "NEAREST"){
					texture_filter = Texture::Filter::NEAREST;
				}else if(strValue == "LINEAR"){
					texture_filter = Texture::Filter::LINEAR;
				}else if(strValue == "BILINEAR"){
					texture_filter = Texture::Filter::BILINEAR;
				}else if(strValue == "TRILINEAR"){
					texture_filter = Texture::Filter::TRILINEAR;
				}else{
					throw CrossException("Unknown Texture Filter %s", strValue.c_str());
				}
			}
			element = element->NextSiblingElement("property");
		}
	}else{
		throw CrossException("XML empty root element");
	}
}
