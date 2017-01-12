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

Config::Config():
	orientation(System::LANDSCAPE),
	use_compressed_textures(false),
	texture_filter(Texture::Filter::NEAREST),
	view_distance(100.f),
	offscreen_render(false)
{
	string gameConfigPath = system->DataPath() + "/GameConfig.xml";
	string userConfigPath = system->DataPath() + "/UserConfig.xml";
	LoadGameConfig(gameConfigPath);
	LoadUserConfig(userConfigPath);
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

string Config::GetString(const string& key, const string& def){
	string strValue = GetString(key);
	if(strValue.empty())
		return def;
	return strValue;
}

S32 Config::GetInt(const string& key, S32 def){
	string strValue = GetString(key);
	if(strValue.empty())
		return def;
	return atoi(strValue.c_str());
}

float Config::GetFloat(const string& key, float def){
	string strValue = GetString(key);
	if(strValue.empty())
		return def;
	return (float)atof(strValue.c_str());
}

bool Config::GetBool(const string& key, bool def){
	return GetInt(key, def) != 0;
}

System::Orientation Config::GetScreenOrientation(){
	return orientation;
}

bool Config::UseCompressedTextures(){
	return use_compressed_textures;
}

Texture::Filter Config::GetTextureFilter(){
	return texture_filter;
}

float Config::GetViewDistance(){
	return view_distance;
}

bool Config::IsOffscreenRender(){
	return offscreen_render;
}

string Config::GetString(const string& key){
	auto entry = user_prefs.find(key);
	if(entry != user_prefs.end()){
		return (*entry).second;
	}else{
		return "";
	}
}

void Config::LoadGameConfig(const string& path){
	/* Code for loading xmls from memery
	TiXmlDocument xml;
	Byte* source = new Byte[xmlFile->size + 1]; // +1 for null terminated string
	memcpy(source, xmlFile->data, xmlFile->size);
	source[xmlFile->size] = 0;
	delete xmlFile;
	xml.Parse((const char*)source, 0, TIXML_ENCODING_UTF8);
	delete source;*/

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

void Config::LoadUserConfig(const string& path){
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
	system->SaveFile(&gameConfig);
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
	system->SaveFile(&userConfig);
	userConfig.data = NULL;
}