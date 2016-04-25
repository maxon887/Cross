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
#include "Light.h"

using namespace cross;

Light::Light(const Vector3D& ambient, const Vector3D& diffuse, const Vector3D& specular) :
	ambient(ambient),
	diffuse(diffuse),
	specular(specular)
{ }

void Light::SetAmbientStrength(const Vector3D& ambient) {
	this->ambient = ambient;
}

void Light::SetDiffuseStrength(const Vector3D& diffuse) {
	this->diffuse = diffuse;
}

void Light::SetSpecularStrength(const Vector3D& specular) {
	this->specular = specular;
}

Vector3D Light::GetAmbientStrength() {
	return ambient;
}

Vector3D Light::GetDiffuseStrength() {
	return diffuse;
}

Vector3D Light::GetSpecularStrength() {
	return specular;
}