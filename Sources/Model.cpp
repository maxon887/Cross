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
#include "Launcher.h"
#include "Mesh.h"
#include "Material.h"

using namespace cross;

Model::Model(Model& obj) : 
	meshes(obj.meshes),
	original(false),
	format(obj.format)
{ }

Model::Model(const string& name) :
	name(name),
	original(true),
	format(UNKNOW)
{ 
	filepath = launcher->PathFromFile(name);
	string extension = launcher->ExtensionFromFile(name);
	if(extension == "fbx" || extension == "FBX"){
		format = FBX;
	}else{
		format = UNKNOW;
	}
}

Model::~Model(){
	if(original){
		for(Mesh* mesh : meshes){
			delete mesh;
		}/*
		for(Material* material : materials){
			delete material;
		}*/
	}
}

void Model::Draw(){
	for(Mesh* mesh : meshes){
		mesh->Draw(GetModelMatrix());
	}
}

string Model::GetName(){
	return name;
}

string Model::GetFilePath(){
	return filepath;
}

Model::Format Model::GetFormat(){
	return format;
}

int Model::GetPolyCount(){
	int polyCount = 0;
	for(Mesh* mesh : meshes){
		polyCount += mesh->GetPrimitivesCount();
	}
	return polyCount;
}

void Model::AddMesh(Mesh* mesh){
	meshes.push_back(mesh);
}
/*
void Model::AddMaterial(Material* material){
	materials.push_back(material);
}*/
/*
Material* Model::GetMaterial(unsigned int index){
	return materials[index];
}*/

void Model::SetMaterial(Material* mat){
	for(Mesh* mesh : meshes){
		mesh->SetMaterial(mat);
	}
}

Model* Model::Clone(){
	return new Model(*this);
}