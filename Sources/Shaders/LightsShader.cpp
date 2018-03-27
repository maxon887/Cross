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
#include "Shaders/LightsShader.h"
#include "System.h"
#include "Light.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"

using namespace cross;

LightsShader::LightsShader() :
	LightsShader("Engine/Shaders/Sources/Light.vtx", "Engine/Shaders/Sources/Light.fgm")
{ }

LightsShader::LightsShader(const String& vert, const String& frag) :
	Shader(vert, frag)
{ }

void LightsShader::Compile(){
	Compile(game->GetCurrentScene()->GetLights());
}

void LightsShader::Compile(const List<Light*>& lights){
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
			CROSS_ASSERT(false, "Unknown light type");
		}
	}

	if(!pointCount && !spotCount && !directionalCount){
		CROSS_FAIL(false, "Current shader required at leas 1 light from scene");
	}

	Compile(pointCount, spotCount, directionalCount);
}

void LightsShader::Compile(U32 pointCount, U32 spotCount, U32 directionalCount){
	uPointLights.clear();
	uDirectionalLights.clear();
	uPointLights.clear();
	AddMacro("DIRECTIONAL_LIGHT_COUNT", directionalCount, true);
	AddMacro("POINT_LIGHT_COUNT", pointCount, true);
	AddMacro("SPOT_LIGHT_COUNT", spotCount, true);

	Shader::Compile();
	for(U32 i = 0; i < pointCount; ++i) {
		String structName = "uPointLights[" + to_string(i) + "]";
		LightUniforms uniforms;
		uniforms.position = glGetUniformLocation(program, String(structName + ".position").ToCStr());
		uniforms.color = glGetUniformLocation(program, String(structName + ".color").ToCStr());
		uniforms.intensity = glGetUniformLocation(program, String(structName + ".intensity").ToCStr());
		uPointLights.push_back(uniforms);
	}

	for(U32 i = 0; i < directionalCount; ++i) {
		String structName = "uDirectionalLights[" + to_string(i) + "]";
		LightUniforms uniforms;
		uniforms.direction = glGetUniformLocation(program, String(structName + ".direction").ToCStr());
		uniforms.color = glGetUniformLocation(program, String(structName + ".color").ToCStr());
		uDirectionalLights.push_back(uniforms);
	}

	for(U32 i = 0; i < spotCount; ++i) {
		String structName = "uSpotLights[" + to_string(i) + "]";
		LightUniforms uniforms;
		uniforms.position = glGetUniformLocation(program, String(structName + ".position").ToCStr());
		uniforms.direction = glGetUniformLocation(program, String(structName + ".direction").ToCStr());
		uniforms.color = glGetUniformLocation(program, String(structName + ".color").ToCStr());
		uniforms.intensity = glGetUniformLocation(program, String(structName + ".intensity").ToCStr());
		uniforms.cut_off = glGetUniformLocation(program, String(structName + ".cut_off").ToCStr());
		uniforms.outer_cut_off = glGetUniformLocation(program, String(structName + ".outer_cut_off").ToCStr());
		uSpotLights.push_back(uniforms);
	}
}

void LightsShader::OnDraw(){
	const List<Light*>& lights = game->GetCurrentScene()->GetLights();
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
			CROSS_ASSERT(false, "Unknown light type");
		}
	}
}