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
#include "Shader.h"
#include "System.h"
#include "File.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

String Shader::Uniform::TypeToString(Type t) {
	switch (t) {
	case Type::INT:
		return "Int";
	case Type::FLOAT:
		return "Float";
	case Type::COLOR:
		return "Color";
	case Type::VEC3:
		return "Vector3D";
	case Type::MAT4:
		return "Matrix";
	case Type::TEXTURE:
		return "Texture";
	case Type::CUBEMAP:
		return "Cubemap";
	default:
		CROSS_RETURN(false, "", "Unknown Shader Uniform type");
	}
}

Shader::Uniform::Type Shader::Uniform::StringToType(const String &type) {
	if(type == "Int") {
		return Type::INT;
	} else if(type == "Float") {
		return Type::FLOAT;
	} else if(type == "Color") {
		return Type::COLOR;
	} else if(type == "Vector3D") {
		return Type::VEC3;
	} else if(type == "Matrix") {
		return Type::MAT4;
	} else if(type == "Texture") {
		return Type::TEXTURE;
	} else if(type == "Cubemap") {
		return Type::CUBEMAP;
	} else {
		CROSS_RETURN(false, Type::UNKNOWN, "Unknown Shader Uniform '#'", type);
	}
}

Shader::Uniform::Uniform(String name, String glName):
	name(std::move(name)),
	glName(std::move(glName))
{ }

Shader::Uniform::Uniform(String name, String glName, Type t) :
	name(std::move(name)),
	glName(std::move(glName)),
	type(t)
{ }

Shader::Uniform::Uniform(String name, String glName, S32 value) :
	name(std::move(name)),
	glName(std::move(glName)),
	type(INT),
	value(value)
{ }

Shader::Uniform::Uniform(String name, String glName, float value) :
	name(std::move(name)),
	glName(std::move(glName)),
	type(FLOAT),
	value(value)
{ }

Shader::Uniform::Uniform(String name, String glName, const Color& value) :
	name(std::move(name)),
	glName(std::move(glName)),
	type(COLOR),
	value(value)
{ }

Shader::Uniform::Uniform(String name, String glName, const Vector3D& value) :
	name(std::move(name)),
	glName(std::move(glName)),
	type(VEC3),
	value(value)
{ }

Shader::Uniform::Uniform(String name, String glName, const Matrix& value) :
	name(std::move(name)),
	glName(std::move(glName)),
	type(MAT4),
	value(value)
{ }

Shader::Uniform::Uniform(String name, String glName, Texture* value) :
	name(std::move(name)),
	glName(std::move(glName)),
	type(TEXTURE),
	value(value)
{ }

Shader::Uniform::Uniform(String name, String glName, Cubemap* value) :
	name(std::move(name)),
	glName(std::move(glName)),
	type(CUBEMAP),
	value(value)
{ }

void Shader::Uniform::SetValue(S32 v) {
	type = INT;
	value.s32 = v;
}

void Shader::Uniform::SetValue(float v) {
	type = FLOAT;
	value.f = v;
}

void Shader::Uniform::SetValue(const Color& v) {
	type = COLOR;
	value.color = v;
}

void Shader::Uniform::SetValue(const Vector3D& v) {
	type = VEC3;
	value.vec3 = v;
}

void Shader::Uniform::SetValue(const Matrix& v) {
	type = MAT4;
	value.mat = v;
}

void Shader::Uniform::SetValue(Texture* v) {
	type = TEXTURE;
	value.texture = v;
}

void Shader::Uniform::SetValue(Cubemap* v) {
	type = CUBEMAP;
	value.cubemap = v;
}

Shader::Uniform::Value& Shader::Uniform::GetValue() {
	return value;
}

Shader::Uniform* Shader::Uniform::Clone() const {
	return CREATE Shader::Uniform(*this);
}

GLint Shader::Uniform::GetID() const {
	return glId;
}

Shader::Shader(const String& vertexFile, const String& fragmentFile) {
	vertex_filename = vertexFile;
	fragment_filename = fragmentFile;
}

Shader::~Shader() {
	FreeResources();
}

void Shader::Save(const String& file, const String& shaderClass) {
	XMLDocument doc;

	XMLElement* shaderXML = doc.NewElement("Shader");
	doc.LinkEndChild(shaderXML);
	
	XMLElement* classXML = doc.NewElement("Class");
	classXML->SetAttribute("name", shaderClass);
	shaderXML->LinkEndChild(classXML);

	XMLElement* vertexXML = doc.NewElement("Vertex");
	vertexXML->SetAttribute("filename", vertex_filename);
	shaderXML->LinkEndChild(vertexXML);

	XMLElement* fragmentXML = doc.NewElement("Fragment");
	fragmentXML->SetAttribute("filename", fragment_filename);
	shaderXML->LinkEndChild(fragmentXML);

	XMLElement* macrosiesXML = doc.NewElement("Macrosies");
	shaderXML->LinkEndChild(macrosiesXML);
	for(const String& macro : macrosies){
		XMLElement* macroXML = doc.NewElement("Macro");
		macroXML->SetText(macro);
		macrosiesXML->LinkEndChild(macroXML);
	}

	XMLElement* uniformsXML = doc.NewElement("Uniforms");
	shaderXML->LinkEndChild(uniformsXML);

	for(const Uniform& uni : uniforms){
		XMLElement* uniformXML = doc.NewElement("Uniform");
		uniformXML->SetAttribute("name", uni.name);
		uniformXML->SetAttribute("glName", uni.glName);
		uniformXML->SetAttribute("type", Uniform::TypeToString(uni.type));
		uniformsXML->LinkEndChild(uniformXML);
	}

	XMLPrinter printer;
	doc.Accept(&printer);
	File saveFile;
	saveFile.name = file;
	saveFile.size = printer.CStrSize() - 1;//-1 because we don't need to save null-terminated string
	saveFile.data = (Byte*)printer.CStr();
	os->SaveFile(&saveFile);
	saveFile.data = nullptr;
}

void Shader::Compile() {
	CROSS_FAIL(vertex_filename != "", "Can not compile shader without vertex file");
	vertex_file = os->LoadAssetFile(vertex_filename);
	vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_file);
	delete vertex_file;
	vertex_file = nullptr;
	CROSS_FAIL(fragment_filename != "", "Can not compile shader without fragment file");
	fragment_file = os->LoadAssetFile(fragment_filename);
	fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_file);
	delete fragment_file;
	fragment_file = nullptr;
	program = glCreateProgram();
	CROSS_FAIL(vertex_shader && fragment_shader, "One or more of shaders files not compiled");
	SAFE(glAttachShader(program, vertex_shader));
	SAFE(glAttachShader(program, fragment_shader));
	CompileProgram();

	aPosition = glGetAttribLocation(program, "aPosition");
	aTexCoords = glGetAttribLocation(program, "aTexCoords");
	aColor = glGetAttribLocation(program, "aColor");
	aNormal = glGetAttribLocation(program, "aNormal");
	aTangent = glGetAttribLocation(program, "aTangent");
	aBitangent = glGetAttribLocation(program, "aBitangent");

	uMVP = glGetUniformLocation(program, "uMVP");
	uModelMatrix = glGetUniformLocation(program, "uModelMatrix");
	uNormalMatrix = glGetUniformLocation(program, "uNormalMatrix");
	uCameraPosition = glGetUniformLocation(program, "uCameraPosition");
	uAmbientLight = glGetUniformLocation(program, "uAmbientLight");

	for(Uniform& uni : uniforms){
		uni.glId = glGetUniformLocation(program, uni.glName);
		CROSS_FAIL(uni.glId != -1, "Uniform # does not contains in the shader", uni.glName);
	}
	compiled = true;
}

void Shader::Use() {
	CROSS_FAIL(IsCompiled(), "Attempt to draw with not compiled shader");
	SAFE(glUseProgram(GetProgram()));
}

bool Shader::IsCompiled() const {
	return compiled;
}

String& Shader::GetFilename(){
	return filename;
}

void Shader::SetFilename(const String& filename) {
	this->filename = filename;
}

String& Shader::GetVertexFilename(){
	return vertex_filename;
}

void Shader::SetVertexFilename(const String& filename) {
	vertex_filename = filename;
}

String& Shader::GetFragmentFilename() {
	return fragment_filename;
}

void Shader::SetFragmentFilename(const String& filename) {
	fragment_filename = filename;
}

void Shader::AddMacro(const String& macro) {
	CROSS_FAIL(!compiled, "Shader already compiled");
	macrosies.Add(macro);
}

void Shader::AddMacro(const String& macro, int value) {
	CROSS_FAIL(!compiled, "Shader already compiled");
	macrosies.Add(macro + " " + String(value));
}

Array<String>& Shader::GetMacrosies() {
	return macrosies;
}

void Shader::AddUniform(String name, String glName) {
	CROSS_FAIL(!compiled, "Can't add uniform to compiled shader");
	uniforms.CreateInside(std::move(name), std::move(glName));
}

void Shader::AddUniform(String name, String glName, Uniform::Type type) {
	CROSS_FAIL(!compiled, "Can't add uniform to compiled shader");
	uniforms.CreateInside(std::move(name), std::move(glName), type);
}

void Shader::AddUniform(String name, String glName, float defValue) {
	CROSS_FAIL(!compiled, "Can't add uniform to compiled shader");
	uniforms.CreateInside(std::move(name), std::move(glName), defValue);
}

void Shader::AddUniform(String name, String glName, const Color& defValue) {
	CROSS_FAIL(!compiled, "Can't add uniform to compiled shader");
	uniforms.CreateInside(std::move(name), std::move(glName), defValue);
}

void Shader::AddUniform(String name, String glName, const Vector3D& defValue) {
	CROSS_FAIL(!compiled, "Can't add uniform to compiled shader");
	uniforms.CreateInside(std::move(name), std::move(glName), defValue);
}

void Shader::AddUniform(String name, String glName, Cubemap* defValue) {
	CROSS_FAIL(!compiled, "Can't add uniform to compiled shader");
	uniforms.CreateInside(std::move(name), std::move(glName), defValue);
}

void Shader::AddUniform(const Uniform& uni) {
	CROSS_FAIL(!compiled, "Can't add uniform to compiled shader");
	uniforms.Add(uni);
}

Shader::Uniform* Shader::GetUniform(const String& name) {
	for(Uniform& uni : uniforms){
		if(uni.name == name){
			return &uni;
		}
	}
	CROSS_RETURN(false, nullptr, "Can not find uniform");
}

Array<Shader::Uniform>& Shader::GetUniforms() {
	return uniforms;
}

GLuint Shader::GetProgram() const {
	return program;
}

GLuint Shader::CompileShader(GLuint type, File* file) {
	CROSS_RETURN(file, 0, "Attempt to compile shader without a file");
	CROSS_RETURN(!compiled, 0, "Shader already compiled");
	String source;
#if defined(IOS) || defined(ANDROID) || defined(GLES)
    if(type == GL_FRAGMENT_SHADER) {
		source += "precision mediump float;\n";
    }
#else
#if defined(WIN)
	if(gfxGL->IsIntelRenderer()) {
		source += "#version " + gfxGL->GetShaderVersion() + "\n";
	}
#endif
#endif

	for(const String& macro : macrosies) {
		source += "#define " + macro + "\n";
	}

	source += String((char*)file->data, (char*)(file->data + file->size));

	GLuint handle = glCreateShader(type);
	char* cptr = source.ToCStr();
	glShaderSource(handle, 1, (const char**)&cptr, nullptr);

	glCompileShader(handle);
	GLint compiled;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
	if(!compiled) {
		GLsizei len;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

		char* log = CREATE char[len + 1];
		glGetShaderInfoLog(handle, len, &len, log);
		CROSS_RETURN(false, 0, "Shader Compilation Failed: #\n#Shader", file->name, log);
	} else {
#ifdef CROSS_DEBUG
		GLsizei len;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
		if(len > 1){
			char* log = CREATE char[len + 1];
			glGetShaderInfoLog(handle, len, &len, log);
			log[len] = 0;
			CROSS_ASSERT(false, "Shader compilation:\n#", log);
			delete[] log;
		}
#endif
	}
	return handle;
}

void Shader::CompileProgram() {
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked) {
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		char* log = CREATE char[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		CROSS_FAIL(false, "Shader program compilation failed:\n #", log);
	}
}

void Shader::FreeResources() {
	uniforms.Clear();
	if(vertex_shader) {
		SAFE(glDeleteShader(vertex_shader));
	}
	if(fragment_shader) {
		SAFE(glDeleteShader(fragment_shader));
	}
	if(program) {
		SAFE(glDeleteProgram(program));
	}
	delete vertex_file;
	delete fragment_file;
}
