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
#include "System.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "Factory.h"
#include "File.h"
#include "Mesh.h"
#include "Material.h"
#include "Shaders/Shader.h"
#include "Shaders/SingleLightShader.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Graphics::Graphics() {
	shader_factory = CREATE Factory<Shader>();
	shader_factory->Register<SingleLightShader>("SingleLightShader");
}

Graphics::~Graphics() {
	delete shader_factory;
}

void Graphics::Start() {
	game->ScreenChanged.Connect(this, &Graphics::OnScreenChanged);
}

void Graphics::Stop() {

}

void Graphics::Update() {
	for(Mesh* mesh : opaque_meshes) {
		mesh->Draw();
	}
	for(Mesh* mesh : transparent_meshes) {
		mesh->Draw();
	}
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
		shader = CREATE Shader();
	}
	shader->SetFilename(shaderfile);

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
	XMLElement* uniformsXML = shaderXML->FirstChildElement("Uniforms");
	if(uniformsXML) {
		XMLElement* uniformXML = uniformsXML->FirstChildElement("Uniform");
		while(uniformXML) {
			const char* name = uniformXML->Attribute("name");
			const char* glName = uniformXML->Attribute("glName");
			const char* type = uniformXML->Attribute("type");
			shader->AddUniform(name, glName, Shader::Uniform::StringToType(type));
			uniformXML = uniformXML->NextSiblingElement("Uniform");
		}
	}

	return shader;
}

void Graphics::OnScreenChanged(Screen* newScreen) {
	opaque_meshes.Clear();
	transparent_meshes.Clear();
	Scene* newScene = dynamic_cast<Scene*>(newScreen);
	if(newScene) {
		newScene->EntityAdded.Connect(this, &Graphics::OnEntityAdded);
		FindAllMeshes(newScene->GetRoot());
	}
}

void Graphics::OnEntityAdded(Entity* newEntity) {
	FindAllMeshes(newEntity);
}

void Graphics::FindAllMeshes(Entity* entity) {
	Mesh* mesh = entity->GetComponent<Mesh>();
	if(mesh) {
		if(mesh->GetMaterial()->IsTransparent()) {
			transparent_meshes.Add(mesh);
		} else {
			opaque_meshes.Add(mesh);
		}
	}
	for(Entity* child : entity->GetChildren()) {
		FindAllMeshes(child);
	}
}
