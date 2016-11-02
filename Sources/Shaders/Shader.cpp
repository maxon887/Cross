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

Shader::Shader(const string& vertexFile, const string& fragmentFile) :
	compiled(false),
	vertex_file(NULL),
	fragment_file(NULL),
	makro_len(0) {
	vertex_file = launcher->LoadFile(vertexFile);
	fragment_file = launcher->LoadFile(fragmentFile);
}

Shader::~Shader(){
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteProgram(program);
	delete vertex_file;
	delete fragment_file;
	for(pair<string, Shader::Property*> pair : properties){
		Shader::Property* prop = pair.second;
		delete prop;
	}
}

bool Shader::UseLights(){
	return false;
}

void Shader::TransferLightData(const CRArray<Light*>& lights){
	throw CrossException("Lighting does not supported by this shader");
}

void Shader::Compile(){
	vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_file);
	delete vertex_file;
	vertex_file = NULL;
	fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_file);
	delete fragment_file;
	fragment_file = NULL;
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
	uColor = glGetUniformLocation(program, "uColor");

	for(pair<string, Shader::Property*> pair : properties){
		Shader::Property* prop = pair.second;
		prop->glId = glGetUniformLocation(program, prop->glName.c_str());
		if(prop->glId != -1){
			properties[prop->name] = prop;
		}else{
			throw CrossException("Property %s does not contains in the shader", prop->glName.c_str());
		}
	}
	compiled = true;
}

bool Shader::IsCompiled(){
	return compiled;
}

void Shader::AddMakro(const string& makro){
	if(compiled){
		throw CrossException("Shader already compiled");
	}
	string makroString = "#define " + makro + "\n";
	macrosies.push_back(makroString);
	makro_len += makroString.length();
}

void Shader::AddMakro(const string& makro, int value){
	if(compiled){
		throw CrossException("Shader already compiled");
	}
	string makroString = "#define " + makro + " " + to_string(value) + "\n";
	macrosies.push_back(makroString);
	makro_len += makroString.length();
}

void Shader::AddProperty(Shader::Property* prop){
	if(!compiled){
		auto it = properties.find(prop->name);
		if(it == properties.end()){
			properties[prop->name] = prop;
		}else{
			throw CrossException("Duplicated property");
		}
	}else{
		throw CrossException("Shader already compiled");
	}
}

void Shader::AddProperty(const string& name, Shader::Property::Type type, const string& glName){
	Property* prop = new Property(name, type, glName);
	AddProperty(prop);
}

Shader::Property* Shader::GetProperty(const string& name){
	return properties[name];
}

GLuint Shader::GetProgram(){
	return program;
}

GLuint Shader::CompileShader(GLuint type, File* file) {
	Byte* source = new Byte[makro_len + file->size + 1]; // +1 for null terminated string

	int curPos = 0;
	for(string makro : macrosies){
		const char* charMakro = makro.c_str();
		memcpy(source + curPos, charMakro, makro.length());
		curPos += makro.length();
	}

	memcpy(source + curPos, file->data, file->size);
	source[makro_len + file->size] = 0;
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
		throw CrossException("Shader: %s\n%sShader", file->name.c_str(), log);
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