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
#include "SpotLight.h"

using namespace cross;

SpotLight::SpotLight(	const Vector3D& ambient,
						const Color& diffuse,
						const Vector3D& specular,
						float constant,
						float linear,
						float quadratic,
						float cutOff ) :
	PointLight(diffuse)
	//cut_off(cutOff)
{
//	Mesh* cone = gfx3D->LoadMesh("Engine/gfx3D/Cone.obj");
	//meshes.push_back(cone);
	SetScale(0.2f);
}

float SpotLight::GetCutOff(){
	return cut_off;
}

float SpotLight::GetOuterCutOff(){
	return cut_off + 10.f;
}