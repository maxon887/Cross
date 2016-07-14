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
#pragma once
#include "Cross.h"
#include "Transformable.h"

namespace cross{

class Light : public Transformable{
public:
	enum Type{
		DIRECTIONAL,
		POINT,
		SPOT
	};

	Light(Type type);
	virtual ~Light(){ };

	Color GetColor() const;
	void SetColor(const Color& color);
	Type GetType() const;
	float GetLinearAttenaution() const;
	float GetQuadraticAttenaution() const;
	void SetCutOff(float cut_off);
	float GetCutOff() const;
	float GetOuterCutOff() const;

protected:
	Type type;
	Color color;
	float cut_off;
	float linear_attenaution;
	float quadratic_attenaution;
};

}