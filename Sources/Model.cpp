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
	Transformable(obj),
	diffuse(obj.diffuse),
	specular(obj.specular),
	shader_type(obj.shader_type),
	color(obj.color),
	meshes(obj.meshes),
	original(false)
{ }

Model::Model(Shader::Type type) :
	Transformable(),
	diffuse(nullptr),
	specular(nullptr),
	shader_type(type),
	color(Color::White),
	original(true),
	meshes()
{ }

Model::~Model(){
	if(original){
		for(Mesh* mesh : meshes){
			delete mesh;
		}
		if(diffuse){
			delete diffuse;
		}
		if(specular){
			delete specular;
		}
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
	case Shader::Type::LIGHT_MATERIAL:
		for(Mesh* mesh : meshes){
			gfx3D->DrawMeshLightMaterial(mesh, GetModelMatrix());
		}
		break;
	case Shader::Type::SIMPLE_POINT_LIGHT:
		if(!diffuse){
			throw CrossException("There are no diffuse texture applied for model");
		}
		if(!specular){
			throw CrossException("There are no specular texture applied for model");
		}
		for(Mesh* mesh : meshes){
			gfx3D->DrawMeshLightMaps(mesh, GetModelMatrix(), diffuse, specular);
		}
		break;
	case Shader::Type::DIRECTIONAL_LIGHT:
		if(!diffuse){
			throw CrossException("There are no diffuse texture applied for model");
		}
		if(!specular){
			throw CrossException("There are no specular texture applied for model");
		}
		for(Mesh* mesh : meshes){
			Vector3D direction(0.f, 0.f, 1.f);
			gfx3D->DrawMeshLightCasters(mesh, GetModelMatrix(), direction, diffuse, specular);
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

void Model::SetSpecularTexture(Texture* specular){
	this->specular = specular;
}

void Model::SetColor(Color color){
	this->color = color;
}

Color Model::GetColor(){
	return color;
}

void Model::SetMaterial(Material material){
	if(shader_type != Shader::LIGHT_MATERIAL){
		throw CrossException("Shader type must be LIGHT");
	}
	for(Mesh* mesh : meshes){
		mesh->SetMaterial(material);
	}
}

Model* Model::Clone(){
	return new Model(*this);
}