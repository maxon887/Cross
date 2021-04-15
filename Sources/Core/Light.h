/*	Copyright © 2018 Maksim Lukyanov

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

/*	Essential Component for visual design. Every scene need to be properly lighted.
	For simulate Sun use DIRECTIONAL light, for bulb like things use POINT light, 
	SPOT light usually good for projector, car light etc */
class Light : public Component {
public:
	/* Type of the Light Component. Describes how light will behave in scene */
	enum Type {
		/* Light spreads by Transforms's direction */
		DIRECTIONAL,
		/* Light spreads in all directions from Transform's position with some coefficient of fading */
		POINT,
		/* Light spreads in cone formed by Transform's position (top of the cone) and Transform's direction */
		SPOT
	};

	Light(Type type = POINT);
	virtual ~Light() { }

	/* Initialize Light Component. Better add all Light to a Scene before any drawings started */
	void Initialize(Scene* scene) override;
	/* Removes Light Component Entity and from all Scene light calculations */
	void Remove() override;
	/* Creates full copy of this Light */
	Light* Clone() const override;

	/* Returns Type of this Light */
	Type GetType() const;

	/* Returns Color of this Light. By default color of the Light absolutely white */
	Color GetColor() const;
	/* Sets Color of this Light. Some ambient effects can be achieved by coloring lights (f.e. some blue Color can be add for moon light)*/
	void SetColor(const Color& color);

	/* Returns Light Intensity. By default 0.01, it around 15 meters for POINT Light */
	float GetIntensity() const;
	/* Sets Light Intensity. Light intensity will affect on how bright objects will be under light and how far it will spread */
	void SetIntensity(float intensity);

	/* Sets cut off angle for SPOT Light in degrees */
	void SetCutOff(float cut_off);
	/* Returns SPOT Light cut off angle */
	float GetCutOff() const;
	/* Returns angle when SPOT Light will be completely cut off */
	float GetOuterCutOff() const;

protected:
	Type type;
	Color color		= Color::White;
	float intensity = 0.01f; //around 15m to max distance
	float cut_off	= 30.f;
};

}