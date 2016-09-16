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
#include "Launcher.h"
#include "File.h"

using namespace cross;

Shader::Shader(const string& vertexFile, const string& fragmentFile) {
	vertex_shader = Compile(vertexFile);
	fragment_shader = Compile(fragmentFile);
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	CompileProgram();

	aPosition = glGetAttribLocation(program, "aPosition");
	aTexCoords = glGetAttribLocation(program, "aTexCoords");
	aNormal = glGetAttribLocation(program, "aNormal");
	
	uMVP = glGetUniformLocation(program, "uMVP");
	uModelMatrix = glGetUniformLocation(program, "uModelMatrix");
	uNormalMatrix = glGetUniformLocation(program, "uNormalMatrix");
	uCameraPosition = glGetUniformLocation(program, "uCameraPosition");
	uAmbientLight = glGetUniformLocation(program, "uAmbientLight");

	active_texture_slot = 0;

	//uColor = glGetUniformLocation(program, "uColor");
	//uSpecularColor = glGetUniformLocation(program, "uSpecularColor");
	//uShininess = glGetUniformLocation(program, "uShininess");
	//uDiffuseTexture = glGetUniformLocation(program, "uDiffuseTexture");
	//uSpecularTexture = glGetUniformLocation(program, "uSpecularTexture");
	//uShininessTexture = glGetUniformLocation(program, "uShininessTexture");
}

Shader::~Shader(){
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteProgram(program);
}

bool Shader::UseLights(){
	return false;
}

void Shader::TransferLightData(const CRArray<Light*>& lights){
	throw CrossException("Lighting does not supported by this shader");
}

void Shader::AddProperty(Shader::Property* prop){
	prop->glId = glGetUniformLocation(program, prop->glName.c_str());
	properices[prop->name] = prop;
}

Shader::Property* Shader::GetProperty(const string& name){
	return properices[name];
}

GLuint Shader::GetProgram(){
	return program;
}

GLuint Shader::Compile(const string& filename){
	//file loading part
		string extension = filename.substr(filename.find_last_of(".") + 1);
		GLuint type;
		if(extension == "vert") {
			type = GL_VERTEX_SHADER;
		} else if(extension == "frag") {
			type = GL_FRAGMENT_SHADER;
		} else {
			throw CrossException("Can't compile shader.\nUnknown file extension.");
		}
		File* file = launcher->LoadFile(filename);
		CRByte* source = new CRByte[file->size + 1]; // +1 for null terminated string
		memcpy(source, file->data, file->size);
		source[file->size] = 0;
		delete file;
		//shader compilling part
		GLuint handle = glCreateShader(type);
		glShaderSource(handle, 1, (const char**)&source, NULL);
		delete[] source;
		source = NULL;

		glCompileShader(handle);
		GLint compiled;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
		if(!compiled) {
			GLsizei len;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

			char* log = new char[len + 1];
			glGetShaderInfoLog(handle, len, &len, log);
			throw CrossException("Shader: %s\n%sShader", filename.c_str(), log);
		} else {
#ifdef CROSS_DEBUG
			GLsizei len;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
			if(len > 1){
				char* log = new char[len + 1];
				glGetShaderInfoLog(handle, len, &len, log);
				string msg(log);
				delete[] log;
				launcher->LogIt("Shader compilation:\n" + msg);
			}
#endif
		}
		return handle;
}

void Shader::CompileProgram(){
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked) {
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		char* log = new char[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		throw CrossException("Shader program compilation failed:\n %s", log);
	}
}