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
#include "Utils/Light.h"

using namespace cross;

MultiLightShader::MultiLightShader() :
	MultiLightShader("Engine/Shaders/multi_light.vert", "Engine/Shaders/multi_light.frag")
{ }

MultiLightShader::MultiLightShader(const string& vert, const string& frag) :
	Shader(vert, frag)
{ }

void MultiLightShader::Compile(){
	Shader::Compile();
	uMaterialDiffuse = glGetUniformLocation(program, "uMaterialDiffuse");
	uMaterialSpecular = glGetUniformLocation(program, "uMaterialSpecular");
	uMaterialShininess = glGetUniformLocation(program, "uMaterialShininess");

	uPointLightCount = glGetUniformLocation(program, "uPointLightCount");
	for(int i = 0; i < MaxPointLights; ++i){
		string structName = "uPointLights[" + to_string(i) + "]";
		uPointLights[i].position = glGetUniformLocation(program, string(structName + ".position").c_str());
		uPointLights[i].color = glGetUniformLocation(program, string(structName + ".color").c_str());
		uPointLights[i].linear = glGetUniformLocation(program, string(structName + ".linear").c_str());
		uPointLights[i].quadratic = glGetUniformLocation(program, string(structName + ".quadratic").c_str());
	}

	uDirectionalLightCount = glGetUniformLocation(program, "uDirectionalLightCount");
	for(int i = 0; i < MaxDirectionalLights; ++i){
		string structName = "uDirectionalLights[" + to_string(i) + "]";
		uDirectionalLights[i].direction = glGetUniformLocation(program, string(structName + ".direction").c_str());
		uDirectionalLights[i].color = glGetUniformLocation(program, string(structName + ".color").c_str());
	}
	
	uSpotLightCount = glGetUniformLocation(program, "uSpotLightCount");
	for(int i = 0; i < MaxSpotLights; ++i){
		string structName = "uSpotLights[" + to_string(i) + "]";
		uSpotLights[i].position = glGetUniformLocation(program, string(structName + ".position").c_str());
		uSpotLights[i].color = glGetUniformLocation(program, string(structName + ".color").c_str());
		uSpotLights[i].linear = glGetUniformLocation(program, string(structName + ".linear").c_str());
		uSpotLights[i].quadratic = glGetUniformLocation(program, string(structName + ".quadratic").c_str());
		uSpotLights[i].cut_off = glGetUniformLocation(program, string(structName + ".cut_off").c_str());
		uSpotLights[i].outer_cut_off = glGetUniformLocation(program, string(structName + ".outer_cut_off").c_str());
		uSpotLights[i].direction = glGetUniformLocation(program, string(structName + ".direction").c_str());
	}
}

bool MultiLightShader::UseLights(){
	return true;
}

void MultiLightShader::TransferLightData(const CRArray<Light*>& lights){
	int pointCount = 0;
	int spotCount = 0;
	int directionalCount = 0;
	for(Light* light : lights){
		switch(light->GetType()) {
		case Light::Type::POINT:{
			SAFE(glUniform3fv(uPointLights[pointCount].position, 1, light->GetPosition().GetData()));
			SAFE(glUniform3fv(uPointLights[pointCount].color, 1, light->GetColor().GetData()));
			SAFE(glUniform1f(uPointLights[pointCount].linear, light->GetIntensity()));
			SAFE(glUniform1f(uPointLights[pointCount].quadratic, light->GetIntensity()));
			pointCount++;
		}break;
		case Light::Type::SPOT:{
			SAFE(glUniform3fv(uSpotLights[spotCount].position, 1, light->GetPosition().GetData()));
			SAFE(glUniform3fv(uSpotLights[spotCount].color, 1, light->GetColor().GetData()));
			SAFE(glUniform1f(uSpotLights[spotCount].linear, light->GetIntensity()));
			SAFE(glUniform1f(uSpotLights[spotCount].quadratic, light->GetIntensity()));
			SAFE(glUniform3fv(uSpotLights[spotCount].direction, 1, light->GetDirection().GetData()));
			SAFE(glUniform1f(uSpotLights[spotCount].cut_off, light->GetCutOff()));
			SAFE(glUniform1f(uSpotLights[spotCount].outer_cut_off, light->GetOuterCutOff()));
			spotCount++;
		}break;
		case Light::Type::DIRECTIONAL:
			SAFE(glUniform3fv(uDirectionalLights[directionalCount].color, 1, light->GetColor().GetData()));
			SAFE(glUniform3fv(uDirectionalLights[directionalCount].direction, 1, light->GetDirection().GetData()));
			directionalCount++;
			break;
		default:
			throw CrossException("Unknown light type");
		}
	}
	SAFE(glUniform1i(uPointLightCount, pointCount));
	SAFE(glUniform1i(uDirectionalLightCount, directionalCount));
	SAFE(glUniform1i(uSpotLightCount, spotCount));
}