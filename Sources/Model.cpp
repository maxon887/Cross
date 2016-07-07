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
#include "Material.h"
#include "Camera.h"
#include "Texture.h"

using namespace cross;

Model::Model(Model& obj) : 
	meshes(obj.meshes),
	original(false)
{ }

Model::Model() :
	meshes(),
	original(true)
{ }

Model::~Model(){
	if(original){
		for(Mesh* mesh : meshes){
			delete mesh;
		}
		for(Material* material : materials){
			delete material;
		}
	}
}

void Model::Draw(){
	for(Mesh* mesh : meshes){
		mesh->Draw(GetModelMatrix());
	}
}

int Model::GetPolyCount(){
	int polyCount = 0;
	for(Mesh* mesh : meshes){
		polyCount += mesh->GetPrimitivesCount();
	}
	return polyCount;
}

Model* Model::Clone(){
	return new Model(*this);
}