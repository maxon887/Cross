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
#include "Graphics3D/Shaders/LightShader.h"

LightShader::LightShader(const string& vertex, const string& fragment) :
	Shader(vertex, fragment)
{ 
	uLightPosition = glGetUniformLocation(program, "uLight.position");
	uLightDirection = glGetUniformLocation(program, "uLight.direction");
	uLightColor = glGetUniformLocation(program, "uLight.color");
}

void LightShader::TransferPointLights(const CRArray<PointLight*>& lights){
	PointLight* light = lights[0];
	if(uLightPosition != -1){
		Vector3D position = light->GetPosition();
		SAFE(glUniform3fv(uLightPosition, 1, position.GetData()));
	}
	if(uLightColor != -1){
		SAFE(glUniform3fv(uLightColor, 1, light->GetColor().GetData()));
	}
}

void LightShader::TransferDirectionLights(const CRArray<DirectionalLight*>& lights){
	DirectionalLight* light = lights[0];
	if(uLightDirection != -1){
		Vector3D direction = light->GetDirection();
		SAFE(glUniform3fv(uLightDirection, 1, direction.GetData()));
	}
	if(uLightColor != -1){
		SAFE(glUniform3fv(uLightColor, 1, light->GetColor().GetData()));
	}
}

bool LightShader::UsePointLights(){
	if(uLightPosition != -1){
		return true;
	}else{
		return false;
	}
}

bool LightShader::UseDirectionalLights(){
	if(uLightDirection != -1){
		return true;
	}else{
		return false;
	}
}