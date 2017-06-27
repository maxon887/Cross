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
#include "System.h"
#include "File.h"
#include "Texture.h"
#include "Utils/Cubemap.h"

using namespace cross;

Shader::Property::Property(const string& name, const string& glName):
	name(name),
	type(UNKNOWN),
	glName(glName),
	glId(-1),
	value(NULL),
	size(0),
	original(true)
{ }

Shader::Property::Property(Shader::Property& obj):
	name(obj.name),
	type(obj.type),
	glName(obj.glName),
	glId(obj.glId),
	size(obj.size),
	original(false)
{
	value = new Byte[obj.size];
	memcpy(value, obj.value, obj.size);
}

Shader::Property::~Property(){
	if(value){
		delete[] value;
	}
}

void Shader::Property::SetValue(U32 v){
	type = INT;
	RealocateIfNeeded(sizeof(U32));
	memcpy(value, &v, size);
}

void Shader::Property::SetValue(float v){
	type = FLOAT;
	RealocateIfNeeded(sizeof(float));
	memcpy(value, &v, size);
}

void Shader::Property::SetValue(const Color& v){
	type = VEC4;
	RealocateIfNeeded(sizeof(Color));
	memcpy(value, v.GetData(), size);
}

void Shader::Property::SetValue(const Vector3D& v){
	type = VEC3;
	RealocateIfNeeded(sizeof(Vector3D));
	memcpy(value, v.GetData(), size);
}

void Shader::Property::SetValue(const Vector4D& v){
	type = VEC4;
	RealocateIfNeeded(sizeof(Vector4D));
	memcpy(value, v.GetData(), size);
}

void Shader::Property::SetValue(Matrix& v){
	type = MAT4;
	RealocateIfNeeded(sizeof(float) * 16);
	memcpy(value, v.GetData(), size);
}

void Shader::Property::SetValue(Texture* v){
	type = SAMPLER;
	GLuint textureID = v->GetID();
	RealocateIfNeeded(sizeof(GLuint));
	memcpy(value, &textureID, size);
}

void Shader::Property::SetValue(Cubemap* cubemap){
	type = CUBEMAP;
	GLuint textureID = cubemap->GetTextureID();
	RealocateIfNeeded(sizeof(GLuint));
	memcpy(value, &textureID, size);
}

Shader::Property* Shader::Property::Clone(){
	return new Shader::Property(*this);
}

GLuint Shader::Property::GetID() const{
	return glId;
}

void Shader::Property::RealocateIfNeeded(U32 newSize){
	if(size < newSize){
		size = newSize;
		if(value){
			delete[] value;
		}
		value = new Byte[newSize];
	}
}

Shader::Shader(const string& vertexFile, const string& fragmentFile) :
	compiled(false),
	vertex_file(NULL),
	fragment_file(NULL),
	makro_len(0)
{
	vertex_file = sys->LoadFile(vertexFile);
	fragment_file = sys->LoadFile(fragmentFile);
	if(gfxGL->GetShaderVersion() >= 130) {
		AddVersion("130");
	}
}

Shader::~Shader(){
	SAFE(glDeleteShader(vertex_shader));
	SAFE(glDeleteShader(fragment_shader));
	SAFE(glDeleteProgram(program));
	delete vertex_file;
	delete fragment_file;
	for(Shader::Property* prop : properties){
		delete prop;
	}
}

bool Shader::UseLights(){
	return false;
}

void Shader::TransferLightData(const List<Light*>& lights){
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
	aTangent = glGetAttribLocation(program, "aTangent");
	aBitangent = glGetAttribLocation(program, "aBitangent");
	
	uMVP = glGetUniformLocation(program, "uMVP");
	uModelMatrix = glGetUniformLocation(program, "uModelMatrix");
	uNormalMatrix = glGetUniformLocation(program, "uNormalMatrix");
	uCameraPosition = glGetUniformLocation(program, "uCameraPosition");
	uAmbientLight = glGetUniformLocation(program, "uAmbientLight");
	uColor = glGetUniformLocation(program, "uColor");

	for(Shader::Property* prop : properties){
		prop->glId = glGetUniformLocation(program, prop->glName.c_str());
		if(prop->glId == -1){
			throw CrossException("Property %s does not contains in the shader", prop->glName.c_str());
		}
	}
	compiled = true;
}

bool Shader::IsCompiled(){
	return compiled;
}

void Shader::AddVersion(const string& ver){
	if(compiled) {
		throw CrossException("Shader already compiled");
	}
	string fullStr = "#version " + ver + "\n";
	macrosies.push_back(fullStr);
	makro_len += fullStr.length();
}

void Shader::AddMacro(const string& makro){
	if(compiled){
		throw CrossException("Shader already compiled");
	}
	string makroString = "#define " + makro + "\n";
	macrosies.push_back(makroString);
	makro_len += makroString.length();
}

void Shader::AddMacro(const string& makro, int value){
	if(compiled){
		throw CrossException("Shader already compiled");
	}
	string makroString = "#define " + makro + " " + to_string(value) + "\n";
	macrosies.push_back(makroString);
	makro_len += makroString.length();
}

void Shader::AddProperty(const string& name, const string& glName){
	AddProperty(new Property(name, glName));
}

void Shader::AddProperty(const string& glName, float defValue){
	AddProperty(glName, glName, defValue);
}

void Shader::AddProperty(const string& name, const string& glName, float defValue){
	Property* prop = new Property(name, glName);
	prop->SetValue(defValue);
	AddProperty(prop);
}

void Shader::AddProperty(const string& name, const string& glName, const Color& defValue) {
	Property* prop = new Property(name, glName);
	prop->SetValue(defValue);
	AddProperty(prop);
}

void Shader::AddProperty(const string& name, const string& glName, const Vector3D& defValue) {
	Property* prop = new Property(name, glName);
	prop->SetValue(defValue);
	AddProperty(prop);
}

void Shader::AddProperty(Shader::Property* prop){
	if(compiled){
		throw CrossException("Shader already compiled");
	}
	if(HaveProperty(prop->name)){
		throw CrossException("Shader already contain that property");
	}
	properties.push_back(prop);
}

Shader::Property* Shader::GetProperty(const string& name){
	for(Property* prop : properties){
		if(prop->name == name){
			return prop;
		}
	}
	throw CrossException("Can not find property");
}

bool Shader::HaveProperty(const string& name){
	for(Property* prop : properties){
		if(prop->name == name){
			return true;
		}
	}
	return false;
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
			log[len] = 0;
			sys->LogIt("Shader compilation:\n%s", log);
			delete[] log;
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