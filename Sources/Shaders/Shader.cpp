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
#include "Shader.h"

using namespace cross;

Shader::Shader(const string& vertexFile, const string& fragmentFile) {
	vertex_shader = gfxGL->ComplileShader(vertexFile);
	fragment_shader = gfxGL->ComplileShader(fragmentFile);
	program = gfxGL->CreateProgram();
	gfxGL->AttachShader(program, vertex_shader);
	gfxGL->AttachShader(program, fragment_shader);
	gfxGL->CompileProgram(program);

	aPosition = glGetAttribLocation(program, "aPosition");
	aTexCoords = glGetAttribLocation(program, "aTexCoords");
	aNormal = glGetAttribLocation(program, "aNormal");

	uMVP = glGetUniformLocation(program, "uMVP");
	uCameraPosition = glGetUniformLocation(program, "uCameraPosition");
	uColor = glGetUniformLocation(program, "uColor");
	uSpecularColor = glGetUniformLocation(program, "uSpecularColor");
	uShininess = glGetUniformLocation(program, "uShininess");
	uDiffuseTexture = glGetUniformLocation(program, "uDiffuseTexture");
	uAmbientLight = glGetUniformLocation(program, "uAmbientLight");
	uModelMatrix = glGetUniformLocation(program, "uModelMatrix");
	uNormalMatrix = glGetUniformLocation(program, "uNormalMatrix");
}

Shader::~Shader(){
	gfxGL->DeleteShader(vertex_shader);
	gfxGL->DeleteShader(fragment_shader);
	gfxGL->DeleteProgram(program);
}

bool Shader::UseDirectionalLights(){
	return false;
}

bool Shader::UsePointLights(){
	return false;
}

bool Shader::UseSpotLights(){
	return false;
}

void Shader::TransferDirectionLights(const CRArray<DirectionalLight*>& lights){
	throw CrossException("Direction lights does not support by this type of shader");
}

void Shader::TransferPointLights(const CRArray<PointLight*>& lights){
	throw CrossException("Point lights does not support by this type of shader");
}

void Shader::TransferSpotLights(const CRArray<SpotLight*> lights){
	throw CrossException("Spot lights does not support by this type of shader");
}