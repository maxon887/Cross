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
#include "Model.h"
#include "Graphics3D.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"

using namespace cross;

Model::Model(Model& obj) : 
	meshes(obj.meshes),
	material(obj.material),
	color(obj.color),
	original(false)
{ }

Model::Model() :
	meshes(),
	material(nullptr),
	original(true)
{ }

Model::Model(CRArray<Mesh*>& meshes, const Color& color) :
	meshes(meshes),
	material(nullptr),
	original(true)
{
	this->color = new Color(color);
}

Model::Model(CRArray<Mesh*>& meshes, const Material& material ) :
	meshes(meshes),
	color(nullptr),
	original(true)
{
	this->material = new Material(material);
}

Model::	Model(CRArray<Mesh*>& meshes) :
	meshes(meshes),
	color(nullptr),
	material(nullptr),
	original(true)
{ }


Model::~Model(){
	if(original){
		for(Mesh* mesh : meshes){
			delete mesh;
		}
		if(color){
			delete color;
		}
		if(material){
			delete material;
		}
	}
}

Color Model::GetColor(){
	if(color != nullptr){
		return *color;
	}else{
		throw CrossException("Current model does not have a color");
	}
}

void Model::SetColor(const Color& c){
	this->color = new Color(c);
}


Material* Model::GetMaterial(){
	if(material != nullptr){
		return material;
	}else{
		throw CrossException("Current model does not have material");
	}
}

void Model::SetDiffuseTexture(Texture* texture){
	for(Mesh* mesh : meshes){
		mesh->SetDiffuseTexture(texture);
	}
}

void Model::SetSpecularTexture(Texture* texture){
	for(Mesh* mesh : meshes){
		mesh->SetSpecularTexture(texture);
	}
}

int Model::GetPolyCount(){
	int polyCount = 0;
	for(Mesh* mesh : meshes){
		polyCount += mesh->GetPolyCount();
	}
	return polyCount;
}

Model* Model::Clone(){
	return new Model(*this);
}