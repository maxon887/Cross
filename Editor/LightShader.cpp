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
#include "LightShader.h"
#include "Utils/Light.h"

#include <math.h>

LightShader::LightShader(const string& vertex, const string& fragment) :
	Shader(vertex, fragment)
{ 
	uLightPosition = glGetUniformLocation(program, "uLight.position");
	uLightDirection = glGetUniformLocation(program, "uLight.direction");
	uLightColor = glGetUniformLocation(program, "uLight.color");
	uLightLinear = glGetUniformLocation(program, "uLight.linear");
	uLightQuadratic = glGetUniformLocation(program, "uLight.quadratic");
	uLightCutOff = glGetUniformLocation(program, "uLight.cut_off");
	uLightOuterCutOff = glGetUniformLocation(program, "uLight.outer_cut_off");
}

bool LightShader::UseLights(){
	return true;
}

void LightShader::TransferLightData(const CRArray<Light*>& lights){
	Light* light = lights[0];

	if(uLightPosition != -1){
		Vector3D position = light->GetPosition();
		SAFE(glUniform3fv(uLightPosition, 1, position.GetData()));
	}
	if(uLightColor != -1){
		SAFE(glUniform3fv(uLightColor, 1, light->GetColor().GetData()));
	}
	if(uLightLinear != -1){
		SAFE(glUniform1f(uLightLinear, light->GetIntensity()));
	}
	if(uLightQuadratic != -1){
		SAFE(glUniform1f(uLightQuadratic, light->GetIntensity()));
	}
	if(uLightLinear != -1){
		SAFE(glUniform1f(uLightLinear, light->GetIntensity()));
	}
	if(uLightQuadratic != -1){
		SAFE(glUniform1f(uLightQuadratic, light->GetIntensity()));
	}
	if(uLightDirection != -1){
		SAFE(glUniform3fv(uLightDirection, 1, light->GetDirection().GetData()));
	}
	if(uLightCutOff != -1){
		SAFE(glUniform1f(uLightCutOff, cos(light->GetCutOff()/180.f * PI)));
	}
	if(uLightOuterCutOff != -1){
		SAFE(glUniform1f(uLightOuterCutOff, cos(light->GetOuterCutOff()/180.f * PI)));
	}
}