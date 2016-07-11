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

Light::Light(Type type):
	type(type),
	color(Color::White),
	linear_attenaution(0.014f),
	quadratic_attenaution(0.0007f),
	cut_off(30.f)
{ }

Color Light::GetColor() const{
	return color;
}

Light::Type Light::GetType() const{
	return type;
}

float Light::GetLinearAttenaution() const{
	return linear_attenaution;
}

float Light::GetQuadraticAttenaution() const{
	return quadratic_attenaution;
}

void Light::SetCutOff(float cut_off){
	this->cut_off = cut_off;
}

float Light::GetCutOff() const{
	return cut_off;
}

float Light::GetOuterCutOff() const{
	return cut_off + 10.f;
}