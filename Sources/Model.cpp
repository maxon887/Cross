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
	type(obj.type),
	meshes(obj.meshes),
	diffuse(obj.diffuse),
	specular(obj.specular),
	material(obj.material),
	color(obj.color),
	original(false)
{ }

Model::Model(Mesh* mesh, const Color& color) :
	Model(Type::SOLID, mesh, &color, nullptr, nullptr, nullptr)
{ }

Model::Model(Mesh* mesh, const Material& material) :
	Model(Type::MATERIAL, mesh, nullptr, &material, nullptr, nullptr)
{ }

Model::Model(Mesh* mesh, Texture* diffuse) :
	Model(Type::TEXTURED, mesh, nullptr, nullptr, diffuse, nullptr)
{ }

Model::Model(Mesh* mesh, Texture* diffuse, Texture* specular) :
	Model(Type::TEXTURED, mesh, nullptr, nullptr, diffuse, specular)
{ }

Model::Model(CRArray<Mesh*>& meshes, const Color& color) :
	Model(Type::SOLID, meshes, &color, nullptr, nullptr, nullptr)
{ }

Model::Model(CRArray<Mesh*>& meshes, const Material& material) :
	Model(Type::MATERIAL, meshes, nullptr, &material, nullptr, nullptr)
{ }

Model::Model(CRArray<Mesh*>& meshes, Texture* diffuse) :
	Model(Type::TEXTURED, meshes, nullptr, nullptr, diffuse, nullptr)
{ }

Model::Model(CRArray<Mesh*>& meshes, Texture* diffuse, Texture* specular) :
	Model(Type::TEXTURED, meshes, nullptr, nullptr, diffuse, specular)
{ }

Model::Model(Type type, Mesh* mesh, const Color* color, const Material* material, Texture* diffuse, Texture* specular) :
	Transformable(),
	type(type),
	meshes(),
	color(nullptr),
	material(nullptr),
	diffuse(diffuse),
	specular(specular),
	original(true)
{
	if(mesh != nullptr){
		meshes.push_back(mesh);
	}
	if(color != nullptr) {
		this->color = new Color(*color);
	}
	if(material != nullptr) {
		this->material = new Material(*material);
	}
}

Model::Model(Type type, CRArray<Mesh*>& meshes, const Color* color, const Material* material, Texture* diffuse, Texture* specular) :
	Transformable(),
	type(type),
	meshes(meshes),
	color(nullptr),
	material(nullptr),
	diffuse(diffuse),
	specular(specular),
	original(true)
{
	if(color != nullptr){
		this->color = new Color(*color);
	}
	if(material != nullptr){
		this->material = new Material(*material);
	}
}


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
		if(color){
			delete color;
		}
		if(material){
			delete material;
		}
	}
}

Model::Type Model::GetType(){
	return type;
}

Color Model::GetColor(){
	if(color != nullptr){
		return *color;
	}else{
		throw CrossException("Current model does not have a color");
	}
}

Texture* Model::GetDiffuseTexture(){
	if(diffuse != nullptr){
		return diffuse;
	}else{
		throw CrossException("Current model does not have diffuse texture");
	}
}

Material* Model::GetMaterial(){
	if(material != nullptr){
		return material;
	}else{
		throw CrossException("Current model does not have material");
	}
}

bool Model::HasSpecularMap(){
	if(specular != nullptr) {
		return true;
	}else{
		return false;
	}
}

Texture* Model::GetSpecularTexture(){
	return specular;
}
/*
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
}*/

int Model::GetPolyCount(){
	int polyCount = 0;
	for(Mesh* mesh : meshes){
		polyCount += mesh->GetPolyCount();
	}
	return polyCount;
}
/*
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
	for(Mesh* mesh : meshes){
		mesh->SetMaterial(material);
	}
}*/

Model* Model::Clone(){
	return new Model(*this);
}