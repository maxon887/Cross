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

PointLight::PointLight(const Color& color) : 
	Light(color)
{ }

PointLight::PointLight(const Color& color, const Vector3D& position) :
	Light(color)
{ 
	SetPosition(position);
}

PointLight::PointLight(const Color& color, const Vector3D& position, float distance) :
	Light(color)
{
	SetPosition(position);
	SetDistance(distance);
}

void PointLight::SetDistance(float distance){
	this->distance = distance;
}