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

Model::Model(Shader::Type type) :
	diffuse(nullptr),
	shader_type(type),
	color(Color::White)
{ }

Model::~Model(){
	for(Mesh* mesh : meshes){
		delete mesh;
	}
	if(diffuse){
		delete diffuse;
	}
}

void Model::Draw(){
	switch(shader_type){
	case Shader::Type::SIMPLE:
		for(Mesh* mesh : meshes){
			gfx3D->DrawMeshSimple(mesh, GetModelMatrix(), color);
		}
		break;
	case Shader::Type::TEXTURE:
		if(!diffuse){
			throw CrossException("There are no diffuse texture applied for model");
		}
		for(Mesh* mesh : meshes){
			gfx3D->DrawMeshTexture(mesh, GetModelMatrix(), diffuse);
		}
		break;
	case Shader::Type::LIGHT:
		for(Mesh* mesh : meshes){
			gfx3D->DrawMeshLight(mesh, GetModelMatrix(), color);
		}
		break;
	default:
		throw CrossException("Wrong shader type");
	}
}

int Model::GetPolyCount(){
	int polyCount = 0;
	for(Mesh* mesh : meshes){
		polyCount += mesh->GetPolyCount();
	}
	return polyCount;
}

void Model::SetMesh(Mesh* mesh){
	meshes.clear();
	meshes.push_back(mesh);
}

void Model::SetMeshes(CRArray<Mesh*>& meshes){
	this->meshes = meshes;
}

void Model::SetDiffuseTexture(Texture* diffuse){
	this->diffuse = diffuse;
}

void Model::SetColor(Color color){
	this->color = color;
}

Color Model::GetColor(){
	return color;
}

void Model::SetMaterial(Material material){
	if(shader_type != Shader::LIGHT){
		throw CrossException("Shader type must be LIGHT");
	}
	for(Mesh* mesh : meshes){
		//mesh->SetMaterial(mate
	}
}