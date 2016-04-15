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
#include "Shaders/LightShader.h"

using namespace cross;

LightShader::LightShader() :
	Shader(Type::LIGHT, "Engine/Shaders/light.vert", "Engine/Shaders/light.frag")
{ 
	uColor = glGetUniformLocation(program, "uColor");
	uAmbientLightStrength = glGetUniformLocation(program, "uAmbientLightStrength");
	uAmbientLightColor = glGetUniformLocation(program, "uAmbientLightColor");
	uLightPosition = glGetUniformLocation(program, "uLightPosition");
	uLightColor = glGetUniformLocation(program, "uLightColor");
	uNormalMatrix = glGetUniformLocation(program, "uNormalMatrix");
	uCameraPosition = glGetUniformLocation(program, "uCameraPosition");

	aNormal = glGetAttribLocation(program, "aNormal");
}