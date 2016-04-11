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

using namespace cross;

Model::Model(CRArray<Mesh*>& meshes) :
	meshes(meshes),
	diffuse(nullptr)
{
}

void Model::Draw(){
	for(Mesh* mesh : meshes){
		gfx3D->DrawMesh(mesh, GetModelMatrix(), diffuse);
	}
}

int Model::GetPolyCount(){
	int polyCount = 0;
	for(Mesh* mesh : meshes){
		polyCount += mesh->poly_count;
	}
	return polyCount;
}

void Model::SetDiffuseTexture(Texture* diffuse){
	this->diffuse = diffuse;
}