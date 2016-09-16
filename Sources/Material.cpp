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
	name("")
{
	for(pair<string, Shader::Property*> pair : shader->properices){
		Shader::Property* prop = pair.second;
		this->properties[prop->name] = nullptr;
	}
}

Material::~Material(){
}

void Material::SetShader(Shader* shader){
	this->shader = shader;
}

void Material::SetPropertyValue(const string& name, void* value){
	properties[name] = value;
}

Shader* Material::GetShader(){
	if(shader){
		return shader;
	}else{
		throw CrossException("Current material does not contain shader");
	}
}
/*
Material* Material::Clone(){
	return new Material(*this);
}*/