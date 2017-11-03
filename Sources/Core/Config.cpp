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
#include "System.h"
#include "File.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Config::Config() {
	LoadGameConfig();
	LoadUserConfig();
}

Config::~Config(){
	SaveGameConfig();
	SaveUserConfig();
}

void Config::SetString(const string& key, const string& value){
	user_prefs[key] = value;
}

void Config::SetInt(const string& key, S32 value){
	SetString(key, to_string(value));
}

void Config::SetFloat(const string& key, float value){
	SetString(key, to_string(value));
}

void Config::SetBool(const string& key, bool value){
	SetInt(key, value);
}

string Config::GetString(const string& key, const string& def) const{
	string strValue = GetString(key);
	if(strValue.empty())
		return def;
	return strValue;
}

S32 Config::GetInt(const string& key, S32 def) const{
	string strValue = GetString(key);
	if(strValue.empty())
		return def;
	return atoi(strValue.c_str());
}

float Config::GetFloat(const string& key, float def) const{
	string strValue = GetString(key);
	if(strValue.empty())
		return def;
	return (float)atof(strValue.c_str());
}

bool Config::GetBool(const string& key, bool def) const{
	return GetInt(key, def) != 0;
}

System::Orientation Config::GetOrientation() const{
	return orientation;
}

void Config::SetOrientation(System::Orientation ori){
	orientation = ori;
}

bool Config::UseCompressedTextures() const{
	return use_compressed_textures;
}

Texture::Filter Config::GetTextureFilter() const{
	return texture_filter;
}

float Config::GetViewDistance() const{
	return view_distance;
}

bool Config::IsOffscreenRender() const{
	return offscreen_render;
}

const string& Config::GetString(const string& key) const{
	auto entry = user_prefs.find(key);
	if(entry != user_prefs.end()){
		return (*entry).second;
	}else{
		return empty;
	}
}

void Config::LoadGameConfig(){
	File* xmlFile = system->LoadAssetFile("GameConfig.xml");
	CROSS_FAIL(xmlFile, "Can not load GameConfig file");
	XMLDocument doc;
	XMLError error = doc.Parse((const char*)xmlFile->data, xmlFile->size);
	CROSS_FAIL(error == XML_SUCCESS, "Can not parse shader xml file");
	delete xmlFile;

	XMLElement* root = doc.FirstChildElement("GameConfig");
	CROSS_FAIL(root, "Failed to find GameConfig root element");
	XMLElement* element = root->FirstChildElement("Property");
	while(element){
		string name = element->Attribute("name");
		string strValue = element->Attribute("value");

		if(name == "Orientation"){
			orientation = (System::Orientation)atoi(strValue.c_str());
		}

		if(name == "UseCompressedTextures"){
			use_compressed_textures = strValue == "true" ? true : false;
		}

		if(name == "TextureFilter"){
			texture_filter = (Texture::Filter)atoi(strValue.c_str());
		}

		if(name == "ViewDistance"){
			view_distance = (float)atof(strValue.c_str());
		}

		if(name == "OffscreenRender"){
			offscreen_render = strValue == "true" ? true : false;
		}

		element = element->NextSiblingElement("Property");
	}
}

void Config::LoadUserConfig(){
    string path = system->DataPath() + "UserConfig.xml";

	XMLDocument doc;
	doc.LoadFile(path.c_str());

	XMLElement* root;
	XMLElement* element;

	root = doc.FirstChildElement("UserConfig");
	CROSS_FAIL(root, "Failed to fined UserConfig root element");
	element = root->FirstChildElement("Property");
	while(element){
		string name = element->Attribute("name");
		string value = element->Attribute("value");
		user_prefs[name] = value;
		element = element->NextSiblingElement("Property");
	}
}

void Config::SaveGameConfig(){
	XMLDocument doc;

	XMLElement* element = doc.NewElement("GameConfig");
	doc.LinkEndChild(element);

	XMLElement* property = doc.NewElement("Property");
	property->SetAttribute("name", "Orientation");
	property->SetAttribute("value", to_string(orientation).c_str());
	element->LinkEndChild(property);

	property = doc.NewElement("Property");
	property->SetAttribute("name", "UseCompressedTextures");
	property->SetAttribute("value", use_compressed_textures ? "true" : "false");
	element->LinkEndChild(property);

	property = doc.NewElement("Property");
	property->SetAttribute("name", "TextureFilter");
	property->SetAttribute("value", to_string(texture_filter).c_str());
	element->LinkEndChild(property);

	property = doc.NewElement("Property");
	property->SetAttribute("name", "ViewDistance");
	property->SetAttribute("value", to_string(view_distance).c_str());
	element->LinkEndChild(property);

	property = doc.NewElement("Property");
	property->SetAttribute("name", "OffscreenRender");
	property->SetAttribute("value", offscreen_render ? "true" : "false");
	element->LinkEndChild(property);

	XMLPrinter printer;
	
	doc.Accept(&printer);
	File gameConfig;
	gameConfig.name = "GameConfig.xml";
	gameConfig.size = printer.CStrSize();
	gameConfig.data = (Byte*)printer.CStr();
	system->SaveDataFile(&gameConfig);
	gameConfig.data = NULL;
}

void Config::SaveUserConfig(){
	XMLDocument doc;

	XMLElement* element = doc.NewElement("UserConfig");
	doc.LinkEndChild(element);

	XMLElement* property;
	for(auto pair : user_prefs){
		property = doc.NewElement("Property");
		property->SetAttribute("name", pair.first.c_str());
		property->SetAttribute("value", pair.second.c_str());
		element->LinkEndChild(property);  
	}
	
	XMLPrinter printer;
	
	doc.Accept(&printer);
	File userConfig;
	userConfig.name = "UserConfig.xml";
	userConfig.size = printer.CStrSize();
	userConfig.data = (Byte*)printer.CStr();
	system->SaveDataFile(&userConfig);
	userConfig.data = NULL;
}