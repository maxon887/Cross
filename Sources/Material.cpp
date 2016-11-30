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
	shader(shader),
	active_texture_slot(0){
	if(shader->IsCompiled()){
		for(Shader::Property* prop : shader->properties){
			properties.push_back(new Shader::Property(*prop));
		}
	}else{
		throw CrossException("Material must use compilded shader");
	}
}

Material::Material(Material& obj) :
	shader(obj.shader),
	active_texture_slot(obj.active_texture_slot)
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
}

Shader::Property* Material::GetProperty(const string& name){
	for(Shader::Property* prop : properties){
		if(prop->name == name){
			return prop;
		}
	}
	throw CrossException("Can not find property");
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

Shader* Material::GetShader(){
	if(shader){
		return shader;
	}else{
		throw CrossException("Current material does not contain shader");
	}
}

Material* Material::Clone(){
	return new Material(*this);
}