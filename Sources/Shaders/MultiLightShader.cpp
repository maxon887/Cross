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
#include "Shaders/MultiLightShader.h"

using namespace cross;

MultiLightShader::MultiLightShader() :
	Shader(Shader::Type::MULTI_LIGHT, "Engine/Shaders/multi_light.vert", "Engine/Shaders/multi_light.frag")
{ 
	uModelMatrix = glGetUniformLocation(program, "uModelMatrix");
	uNormalMatrix = glGetUniformLocation(program, "uNormalMatrix");

	uMaterialDiffuse = glGetUniformLocation(program, "uMaterialDiffuse");
	uMaterialSpecular = glGetUniformLocation(program, "uMaterialSpecular");
	uMaterialShininess = glGetUniformLocation(program, "uMaterialShininess");

	uPointLightCount = glGetUniformLocation(program, "uPointLightCount");
	for(int i = 0; i < MaxPointLights; ++i){
		string structName = "uPointLights[" + to_string(i) + "]";
		uPointLights[i].position = glGetUniformLocation(program, string(structName + ".position").c_str());
		uPointLights[i].ambient = glGetUniformLocation(program, string(structName + ".ambient").c_str());
		uPointLights[i].diffuse = glGetUniformLocation(program, string(structName + ".diffuse").c_str());
		uPointLights[i].specular = glGetUniformLocation(program, string(structName + ".specular").c_str());
		uPointLights[i].constant = glGetUniformLocation(program, string(structName + ".constant").c_str());
		uPointLights[i].linear = glGetUniformLocation(program, string(structName + ".linear").c_str());
		uPointLights[i].quadratic = glGetUniformLocation(program, string(structName + ".quadratic").c_str());
	}

	uCameraPosition = glGetUniformLocation(program, "uCameraPosition");

	aTexCoords = glGetAttribLocation(program, "aTexCoords");
	aNormal = glGetAttribLocation(program, "aNormal");
}