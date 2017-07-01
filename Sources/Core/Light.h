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
#include "Component.h"

namespace cross{

/*	Class needed to light your scene and models. 
	Do not use many light with forward rendering it can make performance issue */
class Light : public Component{
public:
	enum Type{
		DIRECTIONAL,
		POINT,
		SPOT
	};

	Light(Type type);
	virtual ~Light() { };

	virtual void Initialize();
	virtual void Remove();
	virtual Light* Clone();

	Type GetType() const;
	Color GetColor() const;
	void SetColor(const Color& color);
	float GetIntensity() const;
	void SetIntensity(float intensity);
	void SetCutOff(float cut_off);
	float GetCutOff() const;
	float GetOuterCutOff() const;

protected:
	Type type;
	Color color		= Color::White;
	float intensity = 0.01f; //aroud 15m to max distance
	float cut_off	= 30.f;
};

}