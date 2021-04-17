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
#include "Graphics.h"
#include "Factory.h"
#include "System.h"
#include "File.h"
#include "Shaders/Shader.h"
#include "Shaders/SingleLightShader.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Graphics::Graphics() {
	shader_factory = new Factory<Shader>();
	shader_factory->Register<SingleLightShader>("SingleLightShader");
}

Graphics::~Graphics() {
	delete shader_factory;
}

Shader* Graphics::LoadShader(const String& shaderfile) {
	File* xmlFile = os->LoadAssetFile(shaderfile);
	CROSS_RETURN(xmlFile, nullptr, "Can not load shader xml file");
	XMLDocument doc;
	XMLError error = doc.Parse((const char*)xmlFile->data, (Size)xmlFile->size);
	CROSS_RETURN(error == XML_SUCCESS, nullptr, "Can not parse shader xml file");
	delete xmlFile;

	XMLElement* shaderXML = doc.FirstChildElement("Shader");
	CROSS_RETURN(shaderXML, nullptr, "Can not find node Shader in XML file");

	XMLElement* classXML = shaderXML->FirstChildElement("Class");
	Shader* shader = nullptr;
	if(classXML) {
		const char* classname = classXML->Attribute("name");
		shader = shader_factory->Create(classname);
	} else {
		shader = new Shader();
	}

	XMLElement* vertexXML = shaderXML->FirstChildElement("Vertex");
	const char* vertexFile = vertexXML->Attribute("filename");
	XMLElement* fragmentXML = shaderXML->FirstChildElement("Fragment");
	const char* fragmentFile = fragmentXML->Attribute("filename");
	shader->SetVertexFilename(vertexFile);
	shader->SetFragmentFilename(fragmentFile);

	XMLElement* macrosiesXML = shaderXML->FirstChildElement("Macrosies");
	if(macrosiesXML) {
		XMLElement* macroXML = macrosiesXML->FirstChildElement("Macro");
		while(macroXML) {
			const char* text = macroXML->GetText();
			shader->AddMacro(text);
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
			shader->AddProperty(name, glName, Shader::Property::StringToType(type));
			propertyXML = propertyXML->NextSiblingElement("Property");
		}
	}

	return shader;
}