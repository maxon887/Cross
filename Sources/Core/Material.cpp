/*	Copyright © 2018 Maksim Lukyanov

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
#include "Material.h"
#include "System.h"
#include "File.h"
#include "Scene.h"
#include "Game.h"

#include <iomanip>
#include <algorithm>

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Material::Material(Shader* shader) :
	shader(shader)
{
	Reset();
}

Material::Material(const Material& obj) :
	shader(obj.shader),
	active_texture_slot(obj.active_texture_slot),
	transparent(obj.transparent),
	properties(obj.properties)
{ }

void Material::SetShader(Shader* shader){
	this->shader = shader;
	Reset();
}

Shader* Material::GetShader() {
	return shader;
}

const string& Material::GetFilename() const {
	return filename;
}

void Material::Load(const string& filename) {
	File* xmlFile = system->LoadAssetFile(filename);
	CROSS_FAIL(xmlFile, "Can't load material. File not fount");
	XMLDocument doc;
	XMLError error = doc.Parse((const char*)xmlFile->data, xmlFile->size);
	delete xmlFile;
	CROSS_FAIL(error == XML_SUCCESS, "Can't parse material xml file");

	SetName(filename);

	XMLElement* materialXML = doc.FirstChildElement("Material");
	const char* shaderfilename = materialXML->Attribute("shader");
	CROSS_ASSERT(shaderfilename, "Material file not contain 'shader' filename");
	if(shaderfilename) {
		Shader* shader = game->GetCurrentScene()->GetShader(shaderfilename);
		SetShader(shader);
	}

	XMLElement* propertyXML = materialXML->FirstChildElement("Property");
	while(propertyXML) {
		const char* name = propertyXML->Attribute("name");
		CROSS_FAIL(name, "Property without name");
		if(HaveProperty(name)) {
			Shader::Property* prop = GetProperty(name);
			switch(prop->GetType()) {
			case Shader::Property::INT: {
				int value = propertyXML->IntAttribute("value");
				prop->SetValue(value);
			} break;
			case Shader::Property::FLOAT: {
				double value = propertyXML->DoubleAttribute("value");
				prop->SetValue((float)value);
			} break;
			case Shader::Property::COLOR: {
				const char* value = propertyXML->Attribute("value");
				Color color(value);
				prop->SetValue(color);
			} break;
			case Shader::Property::TEXTURE: {
				const char* textureFilename = propertyXML->Attribute("value");
				Texture* texture = game->GetCurrentScene()->GetTexture(textureFilename);
				prop->SetValue(texture);
			} break;
			default:
				CROSS_ASSERT(false, "Unsupported property type");
			}
		}
		propertyXML = propertyXML->NextSiblingElement("Property");
	}

}

void Material::Save(const string& filename) {
	XMLDocument doc;
	XMLElement* materialXML = doc.NewElement("Material");
	if(GetShader()) {
		materialXML->SetAttribute("shader", GetShader()->GetFilename().c_str());
	} else {
		materialXML->SetAttribute("shader", "");
	}
	doc.LinkEndChild(materialXML);

	for(const Shader::Property& prop : properties) {
		XMLElement* propertyXML = doc.NewElement("Property");
		propertyXML->SetAttribute("name", prop.GetName().c_str());
		switch(prop.GetType()) {
		case Shader::Property::Type::COLOR: {
			int rInt = (int)(prop.value.color.R * 255);
			int gInt = (int)(prop.value.color.G * 255);
			int bInt = (int)(prop.value.color.B * 255);
			int aInt = (int)(prop.value.color.A * 255);

			std::stringstream ss;
			ss << std::hex;
			ss << std::setw(2) << std::setfill('0') << rInt;
			ss << std::setw(2) << std::setfill('0') << gInt;
			ss << std::setw(2) << std::setfill('0') << bInt;
			ss << std::setw(2) << std::setfill('0') << aInt;
			string text = ss.str();
			std::transform(text.begin(), text.end(), text.begin(), ::toupper);
			propertyXML->SetAttribute("value", text.c_str());
			break;
		}
		default:
			CROSS_ASSERT(false, "Unknown material property to save");
		}
		materialXML->LinkEndChild(propertyXML);
	}

	XMLPrinter printer;
	doc.Accept(&printer);
	File saveFile;
	saveFile.name = filename;
	saveFile.size = printer.CStrSize();
	saveFile.data = (Byte*)printer.CStr();
	system->SaveFile(&saveFile);
	saveFile.data = NULL;
}

void Material::Reset() {
	properties.clear();
	properties = shader->GetProperties();
}

bool Material::HaveProperty(const string& name) {
	for(const Shader::Property& prop : properties) {
		if(prop.name == name) {
			return true;
		}
	}
	return false;
}

Shader::Property* Material::GetProperty(const string& name){
	for(Shader::Property& prop : properties){
		if(prop.name == name){
			return &prop;
		}
	}
	CROSS_RETURN(false, NULL, "Can not find property '%s'", name.c_str());
}

Shader::Property* Material::GetProperty(U64 glID){
	for(Shader::Property& prop : properties){
		if(prop.glId == glID){
			return &prop;
		}
	}
	CROSS_RETURN(false, NULL, "Can not find property by ID(%d)", glID);
}

Array<Shader::Property>& Material::GetProperties(){
	return properties;
}

void Material::SetPropertyValue(const string& name, S32 value){
	Shader::Property* prop = GetProperty(name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const string& name, float value){
	Shader::Property* prop = GetProperty(name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const string& name, const Color& value){
	Shader::Property* prop = GetProperty(name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const string& name, Vector3D& value){
	Shader::Property* prop = GetProperty(name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const string& name, Vector4D& value){
	Shader::Property* prop = GetProperty(name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const string& name, Matrix& value){
	Shader::Property* prop = GetProperty(name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(const string& name, Texture* value){
	Shader::Property* prop = GetProperty(name);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, S32 value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, float value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, const Color& value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, Vector3D& value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, Vector4D& value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, Matrix& value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(U64 glID, Texture* value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

bool Material::IsTransparent() {
	return transparent;
}

void Material::EnableTransparent(bool yes){
	transparent = yes;
}

Material* Material::Clone() const{
	return new Material(*this);
}

void Material::SetName(const string& name) {
	filename = name;
}