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
#include "Config.h"
#include "Light.h"
#include "Entity.h"
#include "Material.h"
#include "Mesh.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Shaders/LightsShader.h"
#include "File.h"

#include <iomanip>
#include <algorithm>

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

void Scene::Start(){
	Screen::Start();
	is_scene = true;
	root = new Entity();
	root->SetName("Root");

	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, sys->GetAspectRatio(), 0.1f, config->GetViewDistance());
	camera = new Camera(projection);

	resize_del = sys->WindowResized.Connect(this, &Scene::WindowResizeHandle);
}

void Scene::Update(float sec){
	Screen::Update(sec);
	camera->Update(sec);
	root->Update(sec);
}

void Scene::Stop(){
	sys->WindowResized.Disconnect(resize_del);
	for(pair<S32, Shader*> pair : shaders){
		delete pair.second;
	}
	for(pair<S32, Texture*> pair : textures){
		delete pair.second;
	}
	for(pair<S32, Material*> pair : materials){
		delete pair.second;
	}
	for(pair<S32, Model*> pair : models){
		delete pair.second;
	}
	delete camera;
	delete root;
	Screen::Stop();
}

const string& Scene::GetName() const{
	return name;
}

void Scene::SetName(const string& name){
	this->name = name;
}

void Scene::Load(const string& file){
	File* xmlFile = sys->LoadAssetFile(file);
	CROSS_FAIL(xmlFile, "Can not load scene xml file");
	XMLDocument doc;
	XMLError error = doc.Parse((const char*)xmlFile->data, xmlFile->size);
	CROSS_FAIL(error == XML_SUCCESS, "Can not parse shader xml file");
	delete xmlFile;

	XMLElement* scene = doc.FirstChildElement("Scene");
	CROSS_FAIL(scene, "Can not load scene. Root node Scene not found");
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
			Model* model = gfx3D->LoadModel(filename);
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
			LoadEntity(root, objectXML);
			objectXML = objectXML->NextSiblingElement("Object");
		}
	}
}

void Scene::Save(const string& filename){
	XMLDocument doc;

	XMLElement* sceneXML = doc.NewElement("Scene");
	sceneXML->SetAttribute("version", scene_saver_version);
	doc.LinkEndChild(sceneXML);

	//light sorting
	List<Light*>& lights = GetLights();
	U32 pointCount = 0;
	U32 directionCount = 0;
	U32 spotCount = 0;
	for(Light* light : lights){
		switch(light->GetType()) {
		case Light::POINT:
			pointCount++;
			break;
		case Light::DIRECTIONAL:
			break;
			directionCount++;
		case Light::SPOT:
			spotCount++;
			break;
		default:
			break;
		}
	}
	if(pointCount || directionCount || spotCount){
		XMLElement* lightXML = doc.NewElement("Light");
		if(pointCount > 0){
			XMLElement* pointXML = doc.NewElement("Point");
			pointXML->SetAttribute("count", pointCount);
			lightXML->LinkEndChild(pointXML);
		}
		if(directionCount > 0){
			XMLElement* directionalXML = doc.NewElement("Directional");
			directionalXML->SetAttribute("count", directionCount);
			lightXML->LinkEndChild(directionalXML);
		}
		if(spotCount > 0){
			XMLElement* spotXML = doc.NewElement("Spot");
			spotXML->SetAttribute("count", spotCount);
			lightXML->LinkEndChild(spotXML);
		}
		sceneXML->LinkEndChild(lightXML);
	}

	if(textures.size() > 0){
		XMLElement* texturesXML = doc.NewElement("Textures");
		for(pair<S32, Texture*> pair : textures){
			S32 id = pair.first;
			Texture* texture = pair.second;
			XMLElement* textureXML = doc.NewElement("Texture");
			textureXML->SetAttribute("id", id);
			textureXML->SetAttribute("file", texture->GetName().c_str());
			texturesXML->LinkEndChild(textureXML);
		}
		sceneXML->LinkEndChild(texturesXML);
	}

	if(models.size() > 0){
		XMLElement* modelsXML = doc.NewElement("Models");
		for(pair<S32, Model*> pair : models){
			XMLElement* modelXML = doc.NewElement("Model");
			S32 id = pair.first;
			Model* model = pair.second;
			modelXML->SetAttribute("id", id);
			modelXML->SetAttribute("file", model->GetFilename().c_str());
			modelsXML->LinkEndChild(modelXML);
		}
		sceneXML->LinkEndChild(modelsXML);
	}

	if(root->children.size() > 0){
		XMLElement* objectsXML = doc.NewElement("Objects");
		for(Entity* entity : root->children){
			SaveEntity(entity, objectsXML, &doc);
		}
		sceneXML->LinkEndChild(objectsXML);
	}

	XMLPrinter printer;
	doc.Accept(&printer);
	File saveFile;
	saveFile.name = filename;
	saveFile.size = printer.CStrSize();
	saveFile.data = (Byte*)printer.CStr();
	sys->SaveFile(&saveFile);
	saveFile.data = NULL;
}

void Scene::Clear(){
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
	for(pair<S32, Shader*> pair : shaders) {
		delete pair.second;
	}
	shaders.clear();
}

Entity* Scene::GetRoot(){
	return root;
}

void Scene::SetCameraViewDistance(float distance){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, sys->GetAspectRatio(), 0.1f, distance);
	camera->SetProjectionMatrix(projection);
}

Entity* Scene::GetEntity(const string& name){
	Entity* child = root->FindChild(name);
	if(child){
		return child;
	}
	CROSS_RETURN(false, NULL, "Can not find entity %s", name.c_str());
}

void Scene::AddEntity(Entity* entity){
	entity->Initialize();
	root->AddChild(entity);
	EntityAdded(entity);//trigger
}

Entity* Scene::LoadPrimitive(Graphics3D::Primitives primitive){
	switch(primitive) {
	case cross::Graphics3D::CUBE:
		return GetModel("Engine/Models/Cube.obj")->GetHierarchy();
	case cross::Graphics3D::SPHERE:
		return GetModel("Engine/Models/Sphere.obj")->GetHierarchy();
	case cross::Graphics3D::PLANE:
		return GetModel("Engine/Models/Plane.obj")->GetHierarchy();
	default:
		CROSS_RETURN(false, NULL, "Unknown primitive type");
	}
}

Entity* Scene::RemoveEntity(const string& name){
	return root->RemoveChild(name);
}

List<Light*>& Scene::GetLights(){
	return lights;
}

Camera* Scene::GetCamera(){
	return camera;
}

Color Scene::GetAmbientColor() const{
	return ambient_color;
}

Material* Scene::GetMaterial(const string& xmlFile) {
	S32 hash = std::hash<string>{}(xmlFile);
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

Shader* Scene::GetShader(const string& shaderfile) {
	S32 hash = std::hash<string>{}(shaderfile);
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

Texture* Scene::GetTexture(const string& textureFile) {
	S32 hash = std::hash<string>{}(textureFile);
	auto textureIt = textures.find(hash);
	if(textureIt != textures.end()) {
		return (*textureIt).second;
	} else {
		Texture* texture = gfx2D->LoadTexture(textureFile);
		textures[hash] = texture;
		return texture;
	}
}

Model* Scene::GetModel(const string& modelFile) {
	S32 hash = std::hash<string>{}(modelFile);
	auto modelIt = models.find(hash);
	if(modelIt != models.end()) {
		return (*modelIt).second;
	} else {
		Model* model = gfx3D->LoadModel(modelFile);
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

S32 Scene::FindShaderID(Shader* shader){
	for(pair<S32, Shader*> pair : shaders){
		if(pair.second == shader){
			return pair.first;
		}
	}
	CROSS_RETURN(false, -1, "Can not find shader id");
}

S32 Scene::FindTextureID(Texture* texture){
	for(pair<S32, Texture*> pair : textures) {
		if(pair.second->name == texture->name) {
			return pair.first;
		}
	}
	CROSS_RETURN(false, -1, "Can not find texture id");
}

void Scene::LoadEntity(Entity* parent, XMLElement* objectXML) {
	const char* name = objectXML->Attribute("name");
	CROSS_FAIL(name, "Attribute 'name' not contain in Entity node");
	Entity* entity = new Entity();
	parent->AddChild(entity);
	entity->SetName(name);

	XMLElement* posXML = objectXML->FirstChildElement("Position");
	if(posXML) {
		double x = posXML->DoubleAttribute("x");
		double y = posXML->DoubleAttribute("y");
		double z = posXML->DoubleAttribute("z");
		Vector3D pos((float)x, (float)y, (float)z);
		entity->SetPosition(pos);
	}
	XMLElement* rotXML = objectXML->FirstChildElement("Rotation");
	if(rotXML) {
		double x = rotXML->DoubleAttribute("x");
		double y = rotXML->DoubleAttribute("y");
		double z = rotXML->DoubleAttribute("z");
		double angle = rotXML->DoubleAttribute("angle");
		Quaternion rot(Vector3D((float)x, (float)y, (float)z), (float)angle);
		entity->SetRotate(rot);
	}
	XMLElement* scaleXML = objectXML->FirstChildElement("Scale");
	if(scaleXML) {
		double x = scaleXML->DoubleAttribute("x");
		double y = scaleXML->DoubleAttribute("y");
		double z = scaleXML->DoubleAttribute("z");
		Vector3D scale((float)x, (float)y, (float)z);
		entity->SetScale(scale);
	}

	XMLElement* componentsXML = objectXML->FirstChildElement("Components");
	if(componentsXML) {
		XMLElement* meshXML = componentsXML->FirstChildElement("Mesh");
		if(meshXML) {
			S32 id = meshXML->IntAttribute("id");
			const char* modelfilename = meshXML->Attribute("model");

			Model* model = GetModel(modelfilename);
			Mesh* mesh = model->GetMesh(id);

			const char* materialFile = meshXML->Attribute("material");
			if(materialFile){
				Material* mat = GetMaterial(materialFile);
				mesh->SetMaterial(mat);
			}else{
				mesh->SetMaterial(gfx3D->GetDefaultMaterial());
			}
			entity->AddComponent(mesh);
		}
	}

	XMLElement* childrenXML = objectXML->FirstChildElement("Children");
	if(childrenXML) {
		XMLElement* childXML = childrenXML->FirstChildElement("Object");
		while(childXML) {
			LoadEntity(entity, childXML);
			childXML = childXML->NextSiblingElement("Object");
		}
	}
}

Material* Scene::LoadMaterialFromXML(const string& file) {
	File* xmlFile = sys->LoadAssetFile(file);
	CROSS_RETURN(xmlFile, NULL, "Can not load material xml file");
	XMLDocument doc;
	XMLError error = doc.Parse((const char*)xmlFile->data, xmlFile->size);
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
	sys->SaveFile(&saveFile);
	saveFile.data = NULL;
}

void Scene::WindowResizeHandle(S32 width, S32 height){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, sys->GetAspectRatio(), 0.1f, config->GetViewDistance());
	camera->SetProjectionMatrix(projection);
}

void Scene::SaveEntity(Entity* entity, XMLElement* parent, XMLDocument* doc){
	XMLElement* objectXML = doc->NewElement("Object");
	objectXML->SetAttribute("name", entity->GetName().c_str());
	
	XMLElement* posXML = doc->NewElement("Position");
	Vector3D pos = entity->GetPosition();
	posXML->SetAttribute("x", pos.x);
	posXML->SetAttribute("y", pos.y);
	posXML->SetAttribute("z", pos.z);
	objectXML->LinkEndChild(posXML);

	XMLElement* rotXML = doc->NewElement("Rotation");
	Quaternion rot = entity->GetRotate().GetNormalized();
	Vector3D axis = rot.GetAxis();
	float angle = rot.GetAngle();
	rotXML->SetAttribute("x", axis.x);
	rotXML->SetAttribute("y", axis.y);
	rotXML->SetAttribute("z", axis.z);
	rotXML->SetAttribute("angle", angle);
	objectXML->LinkEndChild(rotXML);

	XMLElement* scaleXML = doc->NewElement("Scale");
	Vector3D scale = entity->GetScale();
	scaleXML->SetAttribute("x", scale.x);
	scaleXML->SetAttribute("y", scale.y);
	scaleXML->SetAttribute("z", scale.z);
	objectXML->LinkEndChild(scaleXML);

	Mesh* mesh = entity->GetComponent<Mesh>();
	if(mesh){
		XMLElement* componentsXML = doc->NewElement("Components");
		XMLElement* meshXML = doc->NewElement("Mesh");
		meshXML->SetAttribute("id", mesh->GetID());
		meshXML->SetAttribute("model", mesh->GetModel()->GetFilename().c_str());
		meshXML->SetAttribute("material", mesh->GetMaterial()->GetFilename().c_str());
		componentsXML->LinkEndChild(meshXML);
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
}