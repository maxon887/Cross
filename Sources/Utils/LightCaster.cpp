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
#include "LightCaster.h"
#include "Graphics3D.h"

using namespace cross;

LightCaster::LightCaster(Mesh* mesh, const Vector3D& ambient, const Vector3D& diffuse, const Vector3D& specular) :
	Light(ambient, diffuse, specular),
	Model(mesh, Color::White)
{ }

LightCaster::LightCaster(const Vector3D& ambient, const Vector3D& diffuse, const Vector3D& specular) :
	Light(ambient, diffuse, specular),
	Model(nullptr, Color::White)
{ }

void LightCaster::Draw(){
	for(Mesh* mesh : meshes){
		Vector3D diffuse = Light::diffuse;
		gfx3D->DrawMeshSimple(mesh, GetModelMatrix(), Color(diffuse.x, diffuse.y, diffuse.z));
	}
}