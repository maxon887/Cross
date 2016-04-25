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
#include "Shaders/LightCasterMaterialShader.h"

using namespace cross;

LightCasterMaterialShader::LightCasterMaterialShader() :
	Shader(Type::LIGHT_CASTER_MATERIAL, "Engine/Shaders/light_caster_material.vert", "Engine/Shaders/light_caster_material.frag")
{ 
	uNormalMatrix = glGetUniformLocation(program, "uNormalMatrix");
	uCameraPosition = glGetUniformLocation(program, "uCameraPosition");

	uMaterialAmbient = glGetUniformLocation(program, "uMaterial.ambient");
	uMaterialDiffuse = glGetUniformLocation(program, "uMaterial.diffuse");
	uMaterialSpecular = glGetUniformLocation(program, "uMaterial.specular");
	uMaterialShininess = glGetUniformLocation(program, "uMaterial.shininess");

	uLightPosition = glGetUniformLocation(program, "uLight.position");
	uLightAmbient = glGetUniformLocation(program, "uLight.ambient");
	uLightDiffuse = glGetUniformLocation(program, "uLight.diffuse");
	uLightSpecular = glGetUniformLocation(program, "uLight.specular");

	aNormal = glGetAttribLocation(program, "aNormal");
}