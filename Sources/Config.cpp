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
	File* defaultConfigFile = launcher->LoadFile("GameConfig.xml");
	InitializeGameConfig(defaultConfigFile);
	user_config_path = launcher->DataPath() + "/UserConfig.xml";
	InitializeUserConfig();
}

Config::~Config(){
	SaveFile();
}

void Config::SaveString(string key, string value){
	user_prefs[key] = value;
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
	auto entry = user_prefs.find(key);
	if(entry != user_prefs.end()){
		return (*entry).second;
	}else{
		return "";
	}
}

void Config::InitializeUserConfig(){
	TiXmlDocument doc(user_config_path.c_str());
	doc.LoadFile();

	TiXmlHandle xmlHandle(&doc);
	TiXmlElement* root;
	TiXmlElement* element;

	root = xmlHandle.FirstChildElement("UserConfig").Element();
	if(root){
		element = root->FirstChildElement("Property");
		while(element){
			string name = element->Attribute("name");
			string value = element->Attribute("value");
			user_prefs[name] = value;
			element = element->NextSiblingElement("Property");
		}
	}
}

void Config::InitializeGameConfig(File* xmlFile){
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

	root = xmlDoc.FirstChildElement("GameConfig").Element();
	if(root){
		element = root->FirstChildElement("Property");
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
			element = element->NextSiblingElement("Property");
		}
	}else{
		throw CrossException("XML empty root element");
	}
}

void Config::SaveFile(){
	TiXmlDocument doc;

	TiXmlDeclaration* dec = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(dec);

	TiXmlElement* element = new TiXmlElement("UserConfig");
	doc.LinkEndChild(element);

	TiXmlElement* property;
	for(pair<string, string> pair : user_prefs){
		property = new TiXmlElement( "Property" );  
		element->LinkEndChild( property );  
		property->SetAttribute("name", pair.first.c_str());
		property->SetAttribute("value", pair.second.c_str());
	}

	FILE* userConfigFile = fopen(user_config_path.c_str(), "w");
	doc.SaveFile(userConfigFile);
}