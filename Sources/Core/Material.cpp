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
#include "Material.h"
#include "Shaders/Shader.h"

using namespace cross;

Material::Material(Shader* shader) :
	shader(shader)
{
	Refresh();
}

Material::Material(const Material& obj) :
	shader(obj.shader),
	active_texture_slot(obj.active_texture_slot),
	transparency(obj.transparency)
{ 
	for(Shader::Property* prop : obj.properties){
		properties.push_back(prop->Clone());
	}
}

Material::~Material(){
	for(Shader::Property* prop : properties){
		delete prop;
	}
}

void Material::SetShader(Shader* shader){
	this->shader = shader;
	Refresh();
}

Shader* Material::GetShader() {
	return shader;
}

const string& Material::GetFilename() const {
	return filename;
}

void Material::Refresh() {
	for(Shader::Property* prop : properties){
		delete prop;
	}
	properties.clear();

	for(Shader::Property* prop : shader->properties) {
		properties.push_back(new Shader::Property(*prop));
	}
}

Shader::Property* Material::GetProperty(const string& name){
	for(Shader::Property* prop : properties){
		if(prop->name == name){
			return prop;
		}
	}
	throw CrossException("Can not find property '%s'", name.c_str());
}

Shader::Property* Material::GetProperty(GLuint glID){
	for(Shader::Property* prop : properties){
		if(prop->glId == glID){
			return prop;
		}
	}
	throw CrossException("Can not find property by ID(%d)", glID);
}

Array<Shader::Property*>& Material::GetProperties(){
	return properties;
}

void Material::SetPropertyValue(const string& name, U32 value){
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

void Material::SetPropertyValue(GLuint glID, U32 value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(GLuint glID, float value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(GLuint glID, const Color& value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(GLuint glID, Vector3D& value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(GLuint glID, Vector4D& value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(GLuint glID, Matrix& value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

void Material::SetPropertyValue(GLuint glID, Texture* value){
	Shader::Property* prop = GetProperty(glID);
	prop->SetValue(value);
}

bool Material::HaveProperty(const string& name) {
	for(Shader::Property* prop : properties) {
		if(prop->name == name) {
			return true;
		}
	}
	return false;
}

void Material::TransparencyEnabled(bool yes){
	transparency = yes;
}

Material* Material::Clone() const{
	return new Material(*this);
}

void Material::SetName(const string& name) {
	filename = name;
}