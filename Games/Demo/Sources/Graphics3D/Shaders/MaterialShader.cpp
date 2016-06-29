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
#include "Graphics3D/Shaders/MaterialShader.h"

MaterialShader::MaterialShader() :
	Shader("gfx3D/shaders/material.vert", "Engine/Shaders/material.frag")
{ 
	uMaterialAmbient = glGetUniformLocation(program, "uMaterial.ambient");
	uMaterialDiffuse = glGetUniformLocation(program, "uMaterial.diffuse");
	uMaterialSpecular = glGetUniformLocation(program, "uMaterial.specular");
	uMaterialShininess = glGetUniformLocation(program, "uMaterial.shininess");
	uLightPosition = glGetUniformLocation(program, "uLight.position");
}