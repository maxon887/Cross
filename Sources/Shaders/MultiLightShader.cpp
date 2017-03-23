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
#include "Light.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"

using namespace cross;

MultiLightShader::MultiLightShader() :
	MultiLightShader("Engine/Shaders/multi_light.vert", "Engine/Shaders/multi_light.frag")
{ }

MultiLightShader::MultiLightShader(const string& vert, const string& frag) :
	Shader(vert, frag)
{ }

void MultiLightShader::Compile(){
	Compile(game->GetCurrentScene()->GetLights());
}

void MultiLightShader::Compile(const Array<Light*>& lights){
	int pointCount = 0;
	int spotCount = 0;
	int directionalCount = 0;
	for(Light* light : lights){
		switch(light->GetType()) {
		case Light::Type::POINT:{
			pointCount++;
		}break;
		case Light::Type::SPOT:{
			spotCount++;
		}break;
		case Light::Type::DIRECTIONAL:{
			directionalCount++;
		}break;
		default:
			throw CrossException("Unknown light type");
		}
	}

	if(!pointCount && !spotCount && !directionalCount){
		throw CrossException("Current shader required at leas 1 light from scene");
	}

	AddMakro("DIRECTIONAL_LIGHT_COUNT", directionalCount);
	AddMakro("POINT_LIGHT_COUNT", pointCount);
	AddMakro("SPOT_LIGHT_COUNT", spotCount);

	Shader::Compile();
	for(int i = 0; i < pointCount; ++i){
		string structName = "uPointLights[" + to_string(i) + "]";
		uPointLights[i].position = glGetUniformLocation(program, string(structName + ".position").c_str());
		uPointLights[i].color = glGetUniformLocation(program, string(structName + ".color").c_str());
		uPointLights[i].intensity = glGetUniformLocation(program, string(structName + ".intensity").c_str());
	}

	for(int i = 0; i < directionalCount; ++i){
		string structName = "uDirectionalLights[" + to_string(i) + "]";
		uDirectionalLights[i].direction = glGetUniformLocation(program, string(structName + ".direction").c_str());
		uDirectionalLights[i].color = glGetUniformLocation(program, string(structName + ".color").c_str());
	}
	
	for(int i = 0; i < spotCount; ++i){
		string structName = "uSpotLights[" + to_string(i) + "]";
		uSpotLights[i].position = glGetUniformLocation(program, string(structName + ".position").c_str());
		uSpotLights[i].direction = glGetUniformLocation(program, string(structName + ".direction").c_str());
		uSpotLights[i].color = glGetUniformLocation(program, string(structName + ".color").c_str());
		uSpotLights[i].intensity = glGetUniformLocation(program, string(structName + ".intensity").c_str());
		uSpotLights[i].cut_off = glGetUniformLocation(program, string(structName + ".cut_off").c_str());
		uSpotLights[i].outer_cut_off = glGetUniformLocation(program, string(structName + ".outer_cut_off").c_str());
	}
}

bool MultiLightShader::UseLights(){
	return true;
}

void MultiLightShader::TransferLightData(const Array<Light*>& lights){
	int pointCount = 0;
	int spotCount = 0;
	int directionalCount = 0;
	for(Light* light : lights){
		switch(light->GetType()) {
		case Light::Type::POINT:{
			SAFE(glUniform3fv(uPointLights[pointCount].position, 1, light->GetPosition().GetData()));
			SAFE(glUniform4fv(uPointLights[pointCount].color, 1, light->GetColor().GetData()));
			SAFE(glUniform1f(uPointLights[pointCount].intensity, light->GetIntensity()));
			pointCount++;
		}break;
		case Light::Type::SPOT:{
			SAFE(glUniform3fv(uSpotLights[spotCount].position, 1, light->GetPosition().GetData()));
			SAFE(glUniform4fv(uSpotLights[spotCount].color, 1, light->GetColor().GetData()));
			SAFE(glUniform1f(uSpotLights[spotCount].intensity, light->GetIntensity()));
			SAFE(glUniform3fv(uSpotLights[spotCount].direction, 1, light->GetEntity()->GetDirection().GetData()));
			SAFE(glUniform1f(uSpotLights[spotCount].cut_off, cos(light->GetCutOff() / 180.f * PI)));
			SAFE(glUniform1f(uSpotLights[spotCount].outer_cut_off, cos(light->GetOuterCutOff() / 180.f * PI)));
			spotCount++;
		}break;
		case Light::Type::DIRECTIONAL:
			SAFE(glUniform4fv(uDirectionalLights[directionalCount].color, 1, light->GetColor().GetData()));
			SAFE(glUniform3fv(uDirectionalLights[directionalCount].direction, 1, light->GetEntity()->GetDirection().GetData()));
			directionalCount++;
			break;
		default:
			throw CrossException("Unknown light type");
		}
	}
}