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
#include "PointLight.h"

using namespace cross;

PointLight::PointLight(	const Vector3D& ambient,
						const Vector3D& diffuse,
						const Vector3D& specular,
						float constant,
						float linear,
						float quadratic) : 
	PointLight(ambient, diffuse, specular, constant, linear, quadratic, true)
{ }

PointLight::~PointLight()
{ }

PointLight::PointLight(	const Vector3D& ambient,
						const Vector3D& diffuse,
						const Vector3D& specular,
						float constant,
						float linear,
						float quadratic, 
						bool loadMesh ) :
	LightCaster(ambient, diffuse, specular),
	constant(constant),
	linear(linear),
	quadratic(quadratic)
{ 
	if(loadMesh){
		Mesh* sphere = gfx3D->LoadMesh("Engine/gfx3D/Sphere.obj");
		meshes.push_back(sphere);
		SetScale(0.2f);
	}
}

float PointLight::GetConstant(){
	return constant;
}

float PointLight::GetLinear(){
	return linear;
}

float PointLight::GetQuadratic(){
	return quadratic;
}