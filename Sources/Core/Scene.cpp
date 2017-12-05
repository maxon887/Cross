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
#include "Scene.h"
#include "Camera.h"
#include "Game.h"
#include "System.h"
#include "Light.h"
#include "Entity.h"
#include "Material.h"
#include "Mesh.h"
#include "Shaders/LightsShader.h"
#include "File.h"
#include "Transform.h"
#include "ComponentFactory.h"
#include "Texture.h"

#include <iomanip>
#include <algorithm>

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Scene::Scene() : Screen() {
	is_scene = true;
	root = new Entity("Root");
	root->AddComponent(new Transform());
}

void Scene::Start() {
	Screen::Start();

	system->WindowResized.Connect(this, &Scene::WindowResizeHandle);
}

void Scene::Update(float sec) {
	Screen::Update(sec);
	root->Update(sec);
}

void Scene::Stop() {
	system->WindowResized.Disconnect(this, &Scene::WindowResizeHandle);
	delete root;
	for(pair<S32, Texture*> pair : textures){
		delete pair.second;
	}
	for(pair<S32, Material*> pair : materials){
		delete pair.second;
	}
	for(pair<S32, Model*> pair : models){
		delete pair.second;
	}
	for(pair<S32, Shader*> pair : shaders) {
		delete pair.second;
	}
	Screen::Stop();
}

bool Scene::Load(const string& file) {
	File* xmlFile = system->LoadAssetFile(file);
	CROSS_RETURN(xmlFile, false, "Can not load scene xml file");
	XMLDocument doc;
	XMLError error = doc.Parse((const char*)xmlFile->data, xmlFile->size);
	CROSS_RETURN(error == XML_SUCCESS, false, "Can not parse shader xml file");
	delete xmlFile;

	XMLElement* scene = doc.FirstChildElement("Scene");
	CROSS_RETURN(scene, false, "Can not load scene. Root node Scene not found");
	SetName(File::FileWithoutExtension(file));
	int version = scene->IntAttribute("version");
	CROSS_ASSERT(version <= scene_loader_version, "Scene loader version missmatch");
	//general lighting information
	int pointLightCount = 0;
	int spotLightCount = 0;
	int directionalLightCount = 0;
	XMLElement* lightXML = scene->FirstChildElement("Light");
	if(lightXML){
		XMLElement* pointXML = lightXML->FirstChildElement("Point");
		if(pointXML){
			pointLightCount = pointXML->IntAttribute("count");
		}
		XMLElement* spotXML = lightXML->FirstChildElement("Spot");
		if(spotXML) {
			spotLightCount = spotXML->IntAttribute("count");
		}
		XMLElement* directionalXML = lightXML->FirstChildElement("Directional");
		if(directionalXML) {
			directionalLightCount = directionalXML->IntAttribute("count");
		}
	}
	//models loading
	XMLElement* modelsXML = scene->FirstChildElement("Models");
	if(modelsXML){
		XMLElement* modelXML = modelsXML->FirstChildElement("Model");
		U32 id = modelXML->IntAttribute("id");
		const char* filename = modelXML->Attribute("file");
		if(filename) {
			Model* model = new Model();
			model->Load(filename);
			models[id] = model;
		} else {
			CROSS_ASSERT(false, "Attribute 'filename' not contain in model node");
		}
	}
	//objects loading
	XMLElement* objectsXML = scene->FirstChildElement("Objects");
	if(objectsXML){
		XMLElement* objectXML = objectsXML->FirstChildElement("Object");
		while(objectXML){
			CROSS_RETURN(LoadEntity(root, objectXML), false, "Can't load entity");
			objectXML = objectXML->NextSiblingElement("Object");
		}
	}

	return true;
}

void Scene::Save(const string& filename) {
	XMLDocument doc;

	XMLElement* sceneXML = doc.NewElement("Scene");
	sceneXML->SetAttribute("version", scene_saver_version);
	doc.LinkEndChild(sceneXML);

	//light sorting
	List<Light*>& lights = GetLights();
	U32 pointCount = 0;
	U32 directionCount = 0;
	U32 spotCount = 0;
	for(Light* light : lights) {
		switch(light->GetType()) {
		case Light::POINT:
			pointCount++;
			break;
		case Light::DIRECTIONAL:
            directionCount++;
			break;
		case Light::SPOT:
			spotCount++;
			break;
		default:
			break;
		}
	}
	if(pointCount || directionCount || spotCount) {
		XMLElement* lightXML = doc.NewElement("Light");
		if(pointCount > 0){
			XMLElement* pointXML = doc.NewElement("Point");
			pointXML->SetAttribute("count", pointCount);
			lightXML->LinkEndChild(pointXML);
		}
		if(directionCount > 0) {
			XMLElement* directionalXML = doc.NewElement("Directional");
			directionalXML->SetAttribute("count", directionCount);
			lightXML->LinkEndChild(directionalXML);
		}
		if(spotCount > 0) {
			XMLElement* spotXML = doc.NewElement("Spot");
			spotXML->SetAttribute("count", spotCount);
			lightXML->LinkEndChild(spotXML);
		}
		sceneXML->LinkEndChild(lightXML);
	}

	if(root->children.size() > 0) {
		XMLElement* objectsXML = doc.NewElement("Objects");
		for(Entity* entity : root->children){
			CROSS_FAIL(SaveEntity(entity, objectsXML, &doc), "Can not save entity");
		}
		sceneXML->LinkEndChild(objectsXML);
	}

	XMLPrinter printer;
	doc.Accept(&printer);
	File saveFile;
	saveFile.name = filename;
	saveFile.size = printer.CStrSize();
	saveFile.data = (Byte*)printer.CStr();
	system->SaveFile(&saveFile);
	saveFile.data = NULL;
}

void Scene::Clear() {
	root->RemoveChildren();
	lights.clear();
	for(pair<S32, Material*> pair : materials){
		delete pair.second;
	}
	materials.clear();
	for(pair<S32, Texture*> pair : textures) {
		delete pair.second;
	}
	textures.clear();
}

Entity* Scene::GetRoot() {
	return root;
}

Entity* Scene::GetEntity(const string& name) {
	Entity* child = root->FindChild(name);
	if(child){
		return child;
	}
	CROSS_RETURN(false, NULL, "Can not find entity %s", name.c_str());
}

void Scene::AddEntity(Entity* entity) {
	entity->Initialize();
	root->AddChild(entity);
	EntityAdded(entity);//trigger
}

Entity* Scene::LoadPrimitive(Model::Primitive primitive) {
	switch(primitive) {
	case Model::CUBE:
		return GetModel("Engine/Models/Cube.obj")->GetHierarchy();
	case Model::SPHERE:
		return GetModel("Engine/Models/Sphere.obj")->GetHierarchy();
	case Model::PLANE:
		return GetModel("Engine/Models/Plane.obj")->GetHierarchy();
	default:
		CROSS_RETURN(false, NULL, "Unknown primitive type");
	}
}

Entity* Scene::RemoveEntity(const string& name) {
	return root->RemoveChild(name);
}

List<Light*>& Scene::GetLights() {
	return lights;
}

Camera* Scene::GetCamera() {
	return camera;
}

void Scene::SetCamera(Camera* cam) {
	this->camera = cam;
}

Color Scene::GetAmbientColor() const {
	return ambient_color;
}

Shader* Scene::GetShader(const string& shaderfile) {
	S32 hash = (S32)std::hash<string>{}(shaderfile);
	auto shaderIt = shaders.find(hash);
	if(shaderIt != shaders.end()) {
		return (*shaderIt).second;
	} else {
		Shader* shader = new Shader();
		shader->Load(shaderfile);
		shader->Compile();
		shaders[hash] = shader;
		return shader;
	}
}

Material* Scene::GetMaterial(const string& xmlFile) {
	S32 hash = (S32)std::hash<string>{}(xmlFile);
	auto matIt = materials.find(hash);
	if(matIt != materials.end()) {
		return (*matIt).second;
	} else {
		Material* mat = LoadMaterialFromXML(xmlFile);
		if(mat) {
			materials[hash] = mat;
		}
		return mat;
	}
}

Texture* Scene::GetTexture(const string& textureFile) {
	S32 hash = (S32)std::hash<string>{}(textureFile);
	auto textureIt = textures.find(hash);
	if(textureIt != textures.end()) {
		return (*textureIt).second;
	} else {
		Texture* texture = new Texture();
		texture->Load(textureFile);
		textures[hash] = texture;
		return texture;
	}
}

Model* Scene::GetModel(const string& modelFile) {
	S32 hash = (S32)std::hash<string>{}(modelFile);
	auto modelIt = models.find(hash);
	if(modelIt != models.end()) {
		return (*modelIt).second;
	} else {
		Model* model = new Model();
		model->Load(modelFile);
		models[hash] = model;
		return model;
	}
}

void Scene::RefreshMaterials(){
	for(pair<S32, Material*> pair : materials){
		pair.second->Refresh();
	}
}

void Scene::SetAmbientColor(const Color& color){
	this->ambient_color = color;
}

S32 Scene::FindTextureID(Texture* texture){
	for(pair<S32, Texture*> pair : textures) {
		if(pair.second->name == texture->name) {
			return pair.first;
		}
	}
	CROSS_RETURN(false, -1, "Can not find texture id");
}

bool Scene::LoadEntity(Entity* parent, XMLElement* objectXML) {
	const char* name = objectXML->Attribute("name");
	CROSS_RETURN(name, false, "Attribute 'name' not contain in Entity node");
	Entity* entity = new Entity(name);
	parent->AddChild(entity);

	XMLElement* componentsXML = objectXML->FirstChildElement("Components");
	if(componentsXML) {
		ComponentFactory* factory = game->GetComponentFactory();
		XMLElement* componentXML = componentsXML->FirstChildElement();
		while(componentXML) {
			Component* component = factory->Create(componentXML->Name());
			CROSS_RETURN(component, false, "Can't create component of type %s", componentXML->Name());
			CROSS_RETURN(component->Load(componentXML, this), false, "Can't load component of type '%s'", componentXML->Name());
			entity->AddComponent(component);
			componentXML = componentXML->NextSiblingElement();
		}
	}

	XMLElement* childrenXML = objectXML->FirstChildElement("Children");
	if(childrenXML) {
		XMLElement* childXML = childrenXML->FirstChildElement("Object");
		while(childXML) {
			CROSS_RETURN(LoadEntity(entity, childXML), false, "Can't load child of entity '%s'", parent->GetName().c_str());
			childXML = childXML->NextSiblingElement("Object");
		}
	}

	return true;
}

Material* Scene::LoadMaterialFromXML(const string& file) {
	File* xmlFile = system->LoadAssetFile(file);
	CROSS_RETURN(xmlFile, NULL, "Can not load material xml file");
	XMLDocument doc;
	XMLError error = doc.Parse((const char*)xmlFile->data, xmlFile->size);
	delete xmlFile;
	CROSS_RETURN(error == XML_SUCCESS, NULL, "Can not parse shader xml file");

	Material* material = new Material();
	material->SetName(file);

	XMLElement* materialXML = doc.FirstChildElement("Material");
	const char* shaderfilename = materialXML->Attribute("shader");
	CROSS_ASSERT(shaderfilename, "Material file not contain 'shader' filename");
	if(shaderfilename) {
		Shader* shader = GetShader(shaderfilename);
		material->SetShader(shader);
	}

	XMLElement* propertyXML = materialXML->FirstChildElement("Property");
	while(propertyXML) {
		const char* name = propertyXML->Attribute("name");
		CROSS_RETURN(name, NULL, "Property without name");
		if(material->HaveProperty(name)){
			Shader::Property* prop = material->GetProperty(name);
			switch(prop->GetType())	{
			case Shader::Property::INT:{
				int value = propertyXML->IntAttribute("value");
				prop->SetValue(value);
				} break;
			case Shader::Property::FLOAT:{
				double value = propertyXML->DoubleAttribute("value");
				prop->SetValue((float)value);
				} break;
			case Shader::Property::COLOR:{
				const char* value = propertyXML->Attribute("value");
				Color color(value);
				prop->SetValue(color);
				} break;
			case Shader::Property::SAMPLER:{
				const char* textureFilename = propertyXML->Attribute("value");
				Texture* texture = GetTexture(textureFilename);
				prop->SetValue(texture);
				} break;
			default:
				CROSS_ASSERT(false, "Unsupported property type");
			}
		}
		propertyXML = propertyXML->NextSiblingElement("Property");
	}

	return material;
}

void Scene::SaveMaterialToXML(Material* mat, const string& xmlFile){
	XMLDocument doc;
	XMLElement* materialXML = doc.NewElement("Material");
	if(mat->GetShader()){
		materialXML->SetAttribute("shader", mat->GetShader()->GetFilename().c_str());
	}else{
		materialXML->SetAttribute("shader", "");
	}
	doc.LinkEndChild(materialXML);

	for(Shader::Property* prop : mat->properties) {
		XMLElement* propertyXML = doc.NewElement("Property");
		propertyXML->SetAttribute("name", prop->GetName().c_str());
		switch(prop->GetType())	{
		case Shader::Property::Type::COLOR: {
			Color c(0.f);
			c.SetData((const char*)prop->GetValue());

			int rInt = (int)(c.R * 255);
			int gInt = (int)(c.G * 255);
			int bInt = (int)(c.B * 255);
			int aInt = (int)(c.A * 255);

			std::stringstream ss;
			ss << std::hex;
			ss << std::setw(2) << std::setfill('0') << rInt;
			ss << std::setw(2) << std::setfill('0') << gInt;
			ss << std::setw(2) << std::setfill('0') << bInt;
			ss << std::setw(2) << std::setfill('0') << aInt;
			string text = ss.str();
			std::transform(text.begin(), text.end(), text.begin(), ::toupper);
			propertyXML->SetAttribute("value", text.c_str());
			break;
		}
		default:
			CROSS_ASSERT(false, "Unknown material property to save");
		}
		materialXML->LinkEndChild(propertyXML);
	}

	XMLPrinter printer;
	doc.Accept(&printer);
	File saveFile;
	saveFile.name = xmlFile;
	saveFile.size = printer.CStrSize();
	saveFile.data = (Byte*)printer.CStr();
	system->SaveFile(&saveFile);
	saveFile.data = NULL;
}

void Scene::WindowResizeHandle(S32 width, S32 height){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, system->GetAspectRatio(), 0.1f, camera->GetViewDistance());
	camera->SetProjectionMatrix(projection);
}

bool Scene::SaveEntity(Entity* entity, XMLElement* parent, XMLDocument* doc){
	XMLElement* objectXML = doc->NewElement("Object");
	objectXML->SetAttribute("name", entity->GetName().c_str());

	Array<Component*>& components = entity->GetComponents();
	if(components.size() > 0) {
		XMLElement* componentsXML = doc->NewElement("Components");
		for(Component* component : components) {
			CROSS_RETURN(component->Save(componentsXML, doc), false, "Can't save entity component");
		}
		objectXML->LinkEndChild(componentsXML);
	}

	if(entity->children.size() > 0){
		XMLElement* childrenXML = doc->NewElement("Children");
		for(Entity* child : entity->children){
			SaveEntity(child, childrenXML, doc);
		}
		objectXML->LinkEndChild(childrenXML);
	}
	parent->LinkEndChild(objectXML);
	return true;
}
