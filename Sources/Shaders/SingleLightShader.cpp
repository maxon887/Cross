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
#include "Shaders/SingleLightShader.h"
#include "System.h"
#include "Light.h"
#include "Entity.h"
#include "Game.h"
#include "Scene.h"

using namespace cross;

SingleLightShader::SingleLightShader(const string& vertex, const string& fragment) :
	Shader(vertex, fragment)
{ }

void SingleLightShader::Compile(){
	Shader::Compile();
	light_attribs.position = glGetUniformLocation(program, "uLight.position");
	light_attribs.direction = glGetUniformLocation(program, "uLight.direction");
	light_attribs.color = glGetUniformLocation(program, "uLight.color");
	light_attribs.intensity = glGetUniformLocation(program, "uLight.intensity");
	light_attribs.cut_off = glGetUniformLocation(program, "uLight.cut_off");
	light_attribs.outer_cut_off = glGetUniformLocation(program, "uLight.outer_cut_off");
}

void SingleLightShader::OnDraw(){
	const List<Light*>& lights = game->GetCurrentScene()->GetLights();
	CROSS_FAIL(lights.size() == 1, "Scene must have one light source");
	Light* light = lights.front();

	if(light_attribs.position != -1){
		Vector3D position = light->GetPosition();
		SAFE(glUniform3fv(light_attribs.position, 1, position.GetData()));
	}
	if(light_attribs.color != -1){
		SAFE(glUniform4fv(light_attribs.color, 1, light->GetColor().GetData()));
	}
	if(light_attribs.intensity != -1){
		SAFE(glUniform1f(light_attribs.intensity, light->GetIntensity()));
	}
	if(light_attribs.direction != -1){
		SAFE(glUniform3fv(light_attribs.direction, 1, light->GetEntity()->GetDirection().GetData()));
	}
	if(light_attribs.cut_off != -1){
		SAFE(glUniform1f(light_attribs.cut_off, cos(light->GetCutOff()/180.f * PI)));
	}
	if(light_attribs.outer_cut_off != -1){
		SAFE(glUniform1f(light_attribs.outer_cut_off, cos(light->GetOuterCutOff()/180.f * PI)));
	}
}