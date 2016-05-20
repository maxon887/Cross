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

Shader::Shader(const string& vertexFile, const string& fragmentFile) :
	Shader(Type::NONE, vertexFile, fragmentFile)
{ }

Shader::Shader(Type type, const string& vertexFile, const string& fragmentFile) : 
	type(type)
{
	vertex_shader = gfxGL->ComplileShader(vertexFile);
	fragment_shader = gfxGL->ComplileShader(fragmentFile);
	program = gfxGL->CreateProgram();
	gfxGL->AttachShader(program, vertex_shader);
	gfxGL->AttachShader(program, fragment_shader);
	gfxGL->CompileProgram(program);

	uMVP = glGetUniformLocation(program, "uMVP");
	aPosition = glGetAttribLocation(program, "aPosition");
	aTexCoords = glGetAttribLocation(program, "aTexCoords");
	aNormal = glGetAttribLocation(program, "aNormal");

	for(int i = 0; i < MaxDiffuseMaps; ++i){
		string uniformName = string("uDiffuseMap") + to_string(i);
		uDiffuseMaps[i] = glGetUniformLocation(program, uniformName.c_str());
	}
	for(int i = 0; i < MaxSpecularMaps; ++i){
		string uniformName = string("uSpecularMap") + to_string(i);
		uSpecularMaps[i] = glGetUniformLocation(program, uniformName.c_str());
	}
}

Shader::~Shader(){
	gfxGL->DeleteShader(vertex_shader);
	gfxGL->DeleteShader(fragment_shader);
	gfxGL->DeleteProgram(program);
}