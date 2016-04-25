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
#include "Shaders/LightCasterDiffuseShader.h"

using namespace cross;

LightCasterDiffuseShader::LightCasterDiffuseShader() :
	LightCasterDiffuseShader(Type::LIGHT_CASTER_DIFFUSE, "Engine/Shaders/light_caster_diffuse.vert", "Engine/Shaders/light_caster_diffuse.frag")
{ }

LightCasterDiffuseShader::LightCasterDiffuseShader(Shader::Type type, string vertex, string fragment) :
	Shader(type, vertex, fragment)
{ 
	uNormalMatrix = glGetUniformLocation(program, "uNormalMatrix");
	uCameraPosition = glGetUniformLocation(program, "uCameraPosition");

	uMaterialDiffuse = glGetUniformLocation(program, "uMaterialDiffuse");
	uMaterialSpecular = glGetUniformLocation(program, "uMaterialSpecular");
	uMaterialShininess = glGetUniformLocation(program, "uMaterialShininess");

	uLightPosition = glGetUniformLocation(program, "uLight.position");
	uLightAmbient = glGetUniformLocation(program, "uLight.ambient");
	uLightDiffuse = glGetUniformLocation(program, "uLight.diffuse");
	uLightSpecular = glGetUniformLocation(program, "uLight.specular");

	aNormal = glGetAttribLocation(program, "aNormal");
	aTexCoords = glGetAttribLocation(program, "aTexCoords");
}