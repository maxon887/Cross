#include "Graphics.h"
#include "System.h"
#include "Base/Factory.h"
#include "File.h"
#include "Mesh.h"
#include "Material.h"
#include "Shaders/Shader.h"
#include "Shaders/SingleLightShader.h"
#include "Shaders/MultiLightShader.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Graphics::Graphics() {
	shader_factory = CREATE Factory<Shader>();
	shader_factory->Register<Shader>("Shader");
	shader_factory->Register<SingleLightShader>("SingleLightShader");
	shader_factory->Register<MultiLightShader>("MultiLightShader");
}

Graphics::~Graphics() {
	delete shader_factory;
}

void Graphics::Start() {

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

void Graphics::RegisterMeshForDrawing(Mesh* mesh) {
	Material* mat = mesh->GetMaterial();
	if(mat && mat->IsTransparent()) {
		transparent_meshes.Add(mesh);
	} else {
		opaque_meshes.Add(mesh);
	}
}

void Graphics::UnregisterMeshForDrawing(Mesh* mesh) {
	for(int i = 0; i < opaque_meshes.Size(); i++) {
		if(opaque_meshes[i] == mesh) {
			opaque_meshes.Remove(i);
			return;
		}
	}
	for(int i = 0; i < transparent_meshes.Size(); i++) {
		if(transparent_meshes[i] == mesh) {
			transparent_meshes.Remove(i);
			return;
		}
	}
}