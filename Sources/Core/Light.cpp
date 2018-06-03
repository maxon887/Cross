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
#include "Light.h"
#include "Game.h"
#include "Scene.h"
#include "System.h"

#include  <algorithm>

using namespace cross;

Light::Light(Type type) :
	Component("Light"),
	type(type)
{ }

void Light::Initialize(Scene* scene) {
	List<Light*>& lights = scene->GetLights();
	auto it = std::find(lights.begin(), lights.end(), this);
	CROSS_ASSERT(it == lights.end(), "Current light already in the scene");
	lights.push_back(this);
}

void Light::Remove() {
	Scene* scene = game->GetCurrentScene();
	List<Light*>& lights = scene->GetLights();
	auto it = std::find(lights.begin(), lights.end(), this);
	if(it != lights.end()) {
		lights.erase(it);
	}
}

Color Light::GetColor() const {
	return color;
}

void Light::SetColor(const Color& color) {
	this->color = color;
}

Light* Light::Clone() const {
	return new Light(*this);
}

Light::Type Light::GetType() const {
	return type;
}

float Light::GetIntensity() const {
	return intensity;
}

void Light::SetIntensity(float inten) {
	intensity = inten;
}

void Light::SetCutOff(float cut_off) {
	this->cut_off = cut_off;
}

float Light::GetCutOff() const {
	return cut_off;
}

float Light::GetOuterCutOff() const {
	return cut_off + 10.f;
}