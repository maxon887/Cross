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
#include "Texture.h"
#include "Utils/Cubemap.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Shader::Property::Property(const String& name, const String& glName):
	name(name),
	glName(glName)
{ }

Shader::Property::Property(const String& name, const String& glName, Type t) :
	name(name),
	glName(glName),
	type(t)
{ }

Shader::Property::Property(const String& name, const String& glName, S32 value) :
	name(name),
	glName(glName),
	type(INT),
	value(value)
{ }

Shader::Property::Property(const String& name, const String& glName, float value) :
	name(name),
	glName(glName),
	type(FLOAT),
	value(value)
{ }

Shader::Property::Property(const String& name, const String& glName, const Color& value) :
	name(name),
	glName(glName),
	type(COLOR),
	value(value)
{ }

Shader::Property::Property(const String& name, const String& glName, const Vector3D& value) :
	name(name),
	glName(glName),
	type(VEC3),
	value(value)
{ }

Shader::Property::Property(const String& name, const String& glName, const Matrix& value) :
	name(name),
	glName(glName),
	type(MAT4),
	value(value)
{ }

Shader::Property::Property(const String& name, const String& glName, Texture* value) :
	name(name),
	glName(glName),
	type(TEXTURE),
	value(value)
{ }

Shader::Property::Property(const String& name, const String& glName, Cubemap* value) :
	name(name),
	glName(glName),
	type(CUBEMAP),
	value(value)
{ }

Shader::Property::Property(const Shader::Property& obj):
	name(obj.name),
	type(obj.type),
	glName(obj.glName),
	glId(obj.glId),
	value(obj.value)
{ }

void Shader::Property::SetValue(S32 v) {
	type = INT;
	value.s32 = v;
}

void Shader::Property::SetValue(float v) {
	type = FLOAT;
	value.f = v;
}

void Shader::Property::SetValue(const Color& v) {
	type = COLOR;
	value.color = v;
}

void Shader::Property::SetValue(const Vector3D& v) {
	type = VEC3;
	value.vec3 = v;
}

void Shader::Property::SetValue(const Matrix& v) {
	type = MAT4;
	value.mat = v;
}

void Shader::Property::SetValue(Texture* v) {
	type = TEXTURE;
	value.texture = v;
}

void Shader::Property::SetValue(Cubemap* v) {
	type = CUBEMAP;
	value.cubemap = v;
}

void Shader::Property::SetValue(const Value& v) {
	value = v;
}

const Shader::Property::Value& Shader::Property::GetValue() const {
	return value;
}

Shader::Property* Shader::Property::Clone() const {
	return new Shader::Property(*this);
}

GLuint Shader::Property::GetID() const {
	return glId;
}

Shader::Property::Type Shader::Property::GetType() const {
	return type;
}

const String& Shader::Property::GetName() const {
	return name;
}

const String& Shader::Property::GetGLName() const {
	return glName;
}

Shader::Shader(const String& vertexFile, const String& fragmentFile) {
	vertex_filename = vertexFile;
	fragment_filename = fragmentFile;
}

Shader::~Shader() {
	if(vertex_shader){
		SAFE(glDeleteShader(vertex_shader));
	}
	if(fragment_shader){
		SAFE(glDeleteShader(fragment_shader));
	}	
	if(program){
		SAFE(glDeleteProgram(program));
	}
	delete vertex_file;
	delete fragment_file;
}

void Shader::Load(const String& file) {
	filename = file;
	File* xmlFile = system->LoadAssetFile(file);
	CROSS_FAIL(xmlFile, "Can not load shader xml file");
	XMLDocument doc;
	XMLError error = doc.Parse((const char*)xmlFile->data, xmlFile->size);
	CROSS_FAIL(error == XML_SUCCESS, "Can not parse shader xml file");
	delete xmlFile;

	XMLElement* shaderXML = doc.FirstChildElement("Shader");
	CROSS_FAIL(shaderXML, "Can not find node Shader in XML file");
	XMLElement* vertexXML = shaderXML->FirstChildElement("Vertex");
	const char* vertexFile = vertexXML->Attribute("filename");
	XMLElement* fragmentXML = shaderXML->FirstChildElement("Fragment");
	const char* fragmentFile = fragmentXML->Attribute("filename");
	vertex_filename = vertexFile;
    fragment_filename = fragmentFile;

	XMLElement* macrosiesXML = shaderXML->FirstChildElement("Macrosies");
	if(macrosiesXML) {
		XMLElement* macroXML = macrosiesXML->FirstChildElement("Macro");
		while(macroXML) {
			const char* text = macroXML->GetText();
			AddMacro(text);
			macroXML = macroXML->NextSiblingElement("Macro");
		}
	}
	XMLElement* propertiesXML = shaderXML->FirstChildElement("Properties");
	if(propertiesXML) {
		XMLElement* propertyXML = propertiesXML->FirstChildElement("Property");
		while(propertyXML) {
			const char* name = propertyXML->Attribute("name");
			const char* glName = propertyXML->Attribute("glName");
			const char* type = propertyXML->Attribute("type");

			if(strcmp(type, "Int") == 0) {
				AddProperty(name, glName, Property::INT);
			} else if(strcmp(type, "Float") == 0) {
				AddProperty(name, glName, Property::FLOAT);
			} else if(strcmp(type, "Texture") == 0) {
				AddProperty(name, glName, Property::TEXTURE);
			} else if(strcmp(type, "Color") == 0) {
				AddProperty(name, glName, Property::COLOR);
			} else {
				CROSS_ASSERT(false, "Unknown property type");
			}

			propertyXML = propertyXML->NextSiblingElement("Property");
		}
	}
}

void Shader::Save(const String& file) {
	XMLDocument doc;

	XMLElement* shaderXML = doc.NewElement("Shader");
	doc.LinkEndChild(shaderXML);

	XMLElement* vertexXML = doc.NewElement("Vertex");
	vertexXML->SetAttribute("filename", vertex_filename.c_str());
	shaderXML->LinkEndChild(vertexXML);

	XMLElement* fragmentXML = doc.NewElement("Fragment");
	fragmentXML->SetAttribute("filename", fragment_filename.c_str());
	shaderXML->LinkEndChild(fragmentXML);

	XMLElement* macrosiesXML = doc.NewElement("Macrosies");
	shaderXML->LinkEndChild(macrosiesXML);
	for(const String& macro : user_macro){
		XMLElement* macroXML = doc.NewElement("Macro");
		macroXML->SetText(macro.c_str());
		macrosiesXML->LinkEndChild(macroXML);
	}

	XMLElement* propertiesXML = doc.NewElement("Properties");
	shaderXML->LinkEndChild(propertiesXML);

	for(const Property& prop : properties){
		XMLElement* propertyXML = doc.NewElement("Property");
		propertyXML->SetAttribute("name", prop.GetName().c_str());
		propertyXML->SetAttribute("glName", prop.GetGLName().c_str());
		switch(prop.GetType())	{
		case Property::FLOAT:
			propertyXML->SetAttribute("type", "Float");
			break;
		case Property::INT:
			propertyXML->SetAttribute("type", "Int");
			break;
		case Property::COLOR:
			propertyXML->SetAttribute("type", "Color");
			break;
		case Property::TEXTURE:
			propertyXML->SetAttribute("type", "Texture");
			break;
		default:
			CROSS_ASSERT(false, "Unknown property type to save");
		}
		propertiesXML->LinkEndChild(propertyXML);
	}

	XMLPrinter printer;
	doc.Accept(&printer);
	File saveFile;
	saveFile.name = file;
	saveFile.size = printer.CStrSize();
	saveFile.data = (Byte*)printer.CStr();
	system->SaveFile(&saveFile);
	saveFile.data = nullptr;
}

void Shader::Compile() {
	vertex_file = system->LoadAssetFile(vertex_filename);
	vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_file);
	delete vertex_file;
	vertex_file = nullptr;
	fragment_file = system->LoadAssetFile(fragment_filename);
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
	uColor = glGetUniformLocation(program, "uColor");

	for(Property& prop : properties){
		prop.glId = glGetUniformLocation(program, prop.glName.c_str());
		CROSS_FAIL(prop.glId != -1, "Property %s does not contains in the shader", prop.glName.c_str());
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

void Shader::AddVersion(const String& ver) {
	CROSS_FAIL(!compiled, "Shader already compiled");
	String fullStr = "#version " + ver + " es\n";
	macrosies.push_back(fullStr);
	makro_len += fullStr.length();
}

void Shader::AddMacro(const String& makro, bool system) {
	CROSS_FAIL(!compiled, "Shader already compiled");
	String makroString = "#define " + makro + "\n";
	macrosies.push_back(makroString);
	makro_len += makroString.length();
	if(!system){
		user_macro.push_back(makro);
	}
}

void Shader::AddMacro(const String& makro, int value, bool system) {
	CROSS_FAIL(!compiled, "Shader already compiled");
	String makroString = "#define " + makro + " " + to_string(value) + "\n";
	macrosies.push_back(makroString);
	makro_len += makroString.length();
	if(!system) {
		CROSS_ASSERT(false, "Do not implement yet");
	}
}

Array<String>& Shader::GetMacrosies() {
	return user_macro;
}

void Shader::ClearMacrosies() {
	user_macro.clear();
}

void Shader::AddProperty(const String& name, const String& glName) {
	CROSS_FAIL(!compiled, "Can't add property to compiled shader");
	CROSS_FAIL(!HaveProperty(name), "Shader already contain that property");
	properties.emplace_back(name, glName);
}

void Shader::AddProperty(const String& name, const String& glName, Shader::Property::Type type) {
	CROSS_FAIL(!compiled, "Can't add property to compiled shader");
	CROSS_FAIL(!HaveProperty(name), "Shader already contain that property");
	properties.emplace_back(name, glName, type);
}

void Shader::AddProperty(const String& name, const String& glName, float defValue) {
	CROSS_FAIL(!compiled, "Can't add property to compiled shader");
	CROSS_FAIL(!HaveProperty(name), "Shader already contain that property");
	properties.emplace_back(name, glName, defValue);
}

void Shader::AddProperty(const String& name, const String& glName, const Color& defValue) {
	CROSS_FAIL(!compiled, "Can't add property to compiled shader");
	CROSS_FAIL(!HaveProperty(name), "Shader already contain that property");
	properties.emplace_back(name, glName, defValue);
}

void Shader::AddProperty(const String& name, const String& glName, const Vector3D& defValue) {
	CROSS_FAIL(!compiled, "Can't add property to compiled shader");
	CROSS_FAIL(!HaveProperty(name), "Shader already contain that property");
	properties.emplace_back(name, glName, defValue);
}

void Shader::AddProperty(const String& name, const String& glName, Cubemap* defValue) {
	CROSS_FAIL(!compiled, "Can't add property to compiled shader");
	CROSS_FAIL(!HaveProperty(name), "Shader already contain that property");
	properties.emplace_back(name, glName, defValue);
}

void Shader::AddProperty(const Property& prop) {
	CROSS_FAIL(!compiled, "Can't add property to compiled shader");
	CROSS_FAIL(!HaveProperty(prop.name), "Shader already contain that property");
	properties.push_back(prop);
}

Shader::Property* Shader::GetProperty(const String& name) {
	for(Property& prop : properties){
		if(prop.name == name){
			return &prop;
		}
	}
	CROSS_RETURN(false, nullptr, "Can not find property");
}

Array<Shader::Property>& Shader::GetProperties(){
	return properties;
}

void Shader::ClearProperties() {
	properties.clear();
}

bool Shader::HaveProperty(const String& name) const {
	for(const Property& prop : properties) {
		if(prop.name == name) {
			return true;
		}
	}
	return false;
}

GLuint Shader::GetProgram() const {
	return program;
}

GLuint Shader::CompileShader(GLuint type, File* file) {
	CROSS_RETURN(file, 0, "Attempt to compile shader without a file");
#ifndef MACOS
    if(type == GL_FRAGMENT_SHADER) {
        CROSS_RETURN(!compiled, 0, "Shader already compiled");
        String fullStr = "precision mediump float;\n";
		macrosies.push_back(fullStr);
        makro_len += fullStr.length();
    }
#endif

	Byte* source = new Byte[makro_len + file->size + 1]; // +1 for nullptr terminated String
	int curPos = 0;
	for(String makro : macrosies){
		const char* charMakro = makro.c_str();
		memcpy(source + curPos, charMakro, makro.length());
		curPos += makro.length();
	}

	memcpy(source + curPos, file->data, file->size);
	source[makro_len + file->size] = 0;
	//shader compiling part
	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, (const char**)&source, nullptr);
	delete[] source;
	source = nullptr;

	glCompileShader(handle);
	GLint compiled;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
	if(!compiled) {
		GLsizei len;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

		char* log = new char[len + 1];
		glGetShaderInfoLog(handle, len, &len, log);
		CROSS_RETURN(false, 0, "Shader: %s\n%sShader", file->name.c_str(), log);
	} else {
#ifdef CROSS_DEBUG
		GLsizei len;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
		if(len > 1){
			char* log = new char[len + 1];
			glGetShaderInfoLog(handle, len, &len, log);
			log[len] = 0;
			system->LogIt("Shader compilation:\n%s", log);
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

		char* log = new char[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		CROSS_FAIL(false, "Shader program compilation failed:\n %s", log);
	}
}
