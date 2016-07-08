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
#include "Texture.h"

using namespace cross;

Material::Material(const string& name) : 
	Material(nullptr)
{ 
	this->name = name;
}

Material::Material(Shader* shader) :
	shader(shader),
	diffuse_color(Color::White),
	specular_color(Color::White),
	shininess(0.5f),
	diffuse_texture(nullptr),
	specular_texture(nullptr),
	name("")
{ }

Material::Material(Material& obj) :
	shader(obj.shader),
	diffuse_color(obj.diffuse_color),
	specular_color(obj.specular_color),
	shininess(obj.shininess),
	name("")
{ 
	if(obj.diffuse_texture){
		diffuse_texture = obj.diffuse_texture->Clone();
	}
	if(obj.specular_texture){
		specular_texture = obj.specular_texture->Clone();
	}
}

Material::~Material(){
	delete diffuse_texture;
	delete specular_texture;
}

void Material::SetShader(Shader* shader){
	this->shader = shader;
}

void Material::SetDiffuseColor(const Color& color){
	diffuse_color = color;
}

void Material::SetSpecularColor(const Color& color){
	specular_color = color;
}

void Material::SetDiffuseTexture(Texture* diffuse){
	diffuse_texture = diffuse;
}

void Material::SetSpecularTexture(Texture* specular){
	specular_texture = specular;
}

void Material::SetShininess(float snininess){
	this->shininess = shininess;
}

Color Material::GetDiffuseColor(){
	return diffuse_color;
}

Color Material::GetSpecularColor(){
	return specular_color;
}

float Material::GetShininess(){
	return shininess;
}

Texture* Material::GetDiffuseTexture(){
	if(diffuse_texture){
		return diffuse_texture;
	}else{
		throw CrossException("Current material does not have diffuse texture");
	}
}

Texture* Material::GetSpecularTexture(){
	if(specular_texture){
		return specular_texture;
	}else{
		throw CrossException("Current material does not have specular texture");
	}
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