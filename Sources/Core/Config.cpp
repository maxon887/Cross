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

#include "Libs/TinyXML/tinyxml.h"

using namespace cross;

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
		static const string empty = "";
		return empty;
	}
}

void Config::LoadGameConfig(){
    string path = sys->DataPath() + "/GameConfig.xml";

	TiXmlDocument doc(path.c_str());
	doc.LoadFile();

	TiXmlHandle xmlDoc(&doc);
	TiXmlElement* root;
	TiXmlElement* element;

	root = xmlDoc.FirstChildElement("GameConfig").Element();
	if(root){
		element = root->FirstChildElement("Property");
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
}

void Config::LoadUserConfig(){
    string path = sys->DataPath() + "/UserConfig.xml";

	TiXmlDocument doc(path.c_str());
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

void Config::SaveGameConfig(){
	TiXmlDocument doc;

	TiXmlDeclaration* dec = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(dec);

	TiXmlElement* element = new TiXmlElement("GameConfig");
	doc.LinkEndChild(element);

	TiXmlElement* property = new TiXmlElement("Property");
	property->SetAttribute("name", "Orientation");
	property->SetAttribute("value", to_string(orientation).c_str());
	element->LinkEndChild(property);

	property = new TiXmlElement("Property");
	property->SetAttribute("name", "UseCompressedTextures");
	property->SetAttribute("value", use_compressed_textures ? "true" : "false");
	element->LinkEndChild(property);

	property = new TiXmlElement("Property");
	property->SetAttribute("name", "TextureFilter");
	property->SetAttribute("value", to_string(texture_filter).c_str());
	element->LinkEndChild(property);

	property = new TiXmlElement("Property");
	property->SetAttribute("name", "ViewDistance");
	property->SetAttribute("value", to_string(view_distance).c_str());
	element->LinkEndChild(property);

	property = new TiXmlElement("Property");
	property->SetAttribute("name", "OffscreenRender");
	property->SetAttribute("value", offscreen_render ? "true" : "false");
	element->LinkEndChild(property);

	TiXmlPrinter printer;
	printer.SetIndent("\t");
	
	doc.Accept(&printer);
	File gameConfig;
	gameConfig.name = "GameConfig.xml";
	gameConfig.size = printer.Size();
	gameConfig.data = (Byte*)printer.CStr();
	sys->SaveDataFile(&gameConfig);
	gameConfig.data = NULL;
}

void Config::SaveUserConfig(){
	TiXmlDocument doc;

	TiXmlDeclaration* dec = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(dec);

	TiXmlElement* element = new TiXmlElement("UserConfig");
	doc.LinkEndChild(element);

	TiXmlElement* property;
	for(auto pair : user_prefs){
		property = new TiXmlElement("Property");  
		property->SetAttribute("name", pair.first.c_str());
		property->SetAttribute("value", pair.second.c_str());
		element->LinkEndChild(property);  
	}
	
	TiXmlPrinter printer;
	printer.SetIndent("\t");
	
	doc.Accept(&printer);
	File userConfig;
	userConfig.name = "UserConfig.xml";
	userConfig.size = printer.Size();
	userConfig.data = (Byte*)printer.CStr();
	sys->SaveDataFile(&userConfig);
	userConfig.data = NULL;
}