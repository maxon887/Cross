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
#include "Shaders/MultiLightShader.h"
#include "File.h"

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
	string path = "";
	path = sys->AssetsPath() + file;

	XMLDocument doc;
	XMLError error = doc.LoadFile(path.c_str());
	if(error != XML_SUCCESS){
		if(error == XML_ERROR_FILE_NOT_FOUND){
			throw CrossException("File not found %s", file.c_str());
		}else{
			throw CrossException("Can not parse XML document");
		}
	}

	XMLElement* scene = doc.FirstChildElement("Scene");
	if(scene){
		name = scene->Attribute("name");
		SetName(name);
		int version = scene->IntAttribute("version");
		if(version > scene_loader_version){
			throw CrossException("Scene loader version missmatch");
		}
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
		//shaders loading
		XMLElement* shadersXML = scene->FirstChildElement("Shaders");
		if(shadersXML){
			XMLElement* shaderXML = shadersXML->FirstChildElement("Shader");
			while(shaderXML){
				MultiLightShader* shader = NULL;
				int id = shaderXML->IntAttribute("id");
				const char* useLightsStr = shaderXML->Attribute("multiLight");
				bool multiLight = strcmp(useLightsStr, "true") == 0;
				if(multiLight){
					shader = new MultiLightShader();
				}else{
					throw CrossException("Do not implement yet");
				}
				XMLElement* macrosies = shaderXML->FirstChildElement("Macrosies");
				if(macrosies){
					XMLElement* macro = macrosies->FirstChildElement("Macro");
					while(macro){
						const char* text = macro->GetText();
						shader->AddMacro(text);
						macro = macro->NextSiblingElement("Macro");
					}
				}
				XMLElement* properties = shaderXML->FirstChildElement("Properties");
				if(properties){
					XMLElement* property = properties->FirstChildElement("Property");
					while(property){
						const char* name = property->Attribute("name");
						const char* glName = property->Attribute("glName");
						const char* defVal = property->Attribute("default");
						if(defVal){
							float def = (float)atof(defVal);
							shader->AddProperty(name, glName, def);
						}else{
							shader->AddProperty(name, glName);
						}
						property = property->NextSiblingElement("Property");
					}
				}

				shader->Compile(pointLightCount, spotLightCount, directionalLightCount);
				shaders[id] = shader;
				shaderXML = shaderXML->NextSiblingElement("Shader");
			}
		}
		//textures loading
		XMLElement* texturesXML = scene->FirstChildElement("Textures");
		if(texturesXML){
			XMLElement* textureXML = texturesXML->FirstChildElement("Texture");
			while(textureXML){
				int id = textureXML->IntAttribute("id");
				const char* file = textureXML->Attribute("file");
				Texture* texture = gfx2D->LoadTexture(file);
				textures[id] = texture;
				textureXML = textureXML->NextSiblingElement("Texture");
			}
		}
		//materials loading
		XMLElement* materialsXML = scene->FirstChildElement("Materials");
		if(materialsXML){
			XMLElement* materialXML = materialsXML->FirstChildElement("Material");
			while(materialXML){
				int id = materialXML->IntAttribute("id");
				int shaderID = materialXML->IntAttribute("shader");
				Material* material = new Material(shaders[shaderID]);
				XMLElement* property = materialXML->FirstChildElement("Property");
				while(property){
					const char* type = property->Attribute("type");
					const char* name = property->Attribute("name");

					if(strcmp(type, "Texture") == 0){
						int textureID = property->IntAttribute("value");
						material->SetPropertyValue(name, textures[textureID]);
					}else if(strcmp(type, "Float") == 0){
						double val = property->DoubleAttribute("value");
						material->SetPropertyValue(name, (float)val);
					}else if(strcmp(type, "Color") == 0){
						XMLElement* colorXML = property->FirstChildElement("Color");
						double r = colorXML->DoubleAttribute("r");
						double g = colorXML->DoubleAttribute("g");
						double b = colorXML->DoubleAttribute("b");
						double a = colorXML->DoubleAttribute("a");
						Color c((float)r, (float)g, (float)b, (float)a);
						material->SetPropertyValue(name, c);
					}else{
						throw CrossException("Unknown material property type");
					}

					property = property->NextSiblingElement("Property");
				}

				materials[id] = material;
				materialXML = materialXML->NextSiblingElement("Material");
			}
		}
		//models loading
		XMLElement* modelsXML = scene->FirstChildElement("Models");
		if(modelsXML){
			XMLElement* modelXML = modelsXML->FirstChildElement("Model");
			U32 id = modelXML->IntAttribute("id");
			const char* filename = modelXML->Attribute("file");
			Model* model = gfx3D->LoadModel(filename);
			models[id] = model;
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
	}else{
		throw CrossException("Can not load scene. Wrong file format");
	}
}

void Scene::Save(const string& filename){
	XMLDocument doc;

	XMLElement* sceneXML = doc.NewElement("Scene");
	sceneXML->SetAttribute("name", GetName().c_str());
	sceneXML->SetAttribute("version", scene_loader_version);
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

	if(shaders.size() > 0){
		XMLElement* shadersXML = doc.NewElement("Shaders");
		for(pair<S32, Shader*> pair : shaders){
			S32 id = pair.first;
			Shader* shader = pair.second;
			XMLElement* shaderXML= doc.NewElement("Shader");
			shaderXML->SetAttribute("id", id);
			if(dynamic_cast<MultiLightShader*>(shader)){
				shaderXML->SetAttribute("multiLight", true);
			}
			if(shader->macrosies.size() > 0){
				XMLElement* macrosiesXML = doc.NewElement("Macrosies");
				for(const string& macro : shader->user_macro){
					XMLElement* macroXML = doc.NewElement("Macro");
					macroXML->SetText(macro.c_str());
					macrosiesXML->LinkEndChild(macroXML);
				}
				shaderXML->LinkEndChild(macrosiesXML);
				XMLElement* propertiesXML = doc.NewElement("Properties");
				for(Shader::Property* prop : shader->properties){
					XMLElement* propertyXML = doc.NewElement("Property");
					propertyXML->SetAttribute("name", prop->name.c_str());
					propertyXML->SetAttribute("glName", prop->glName.c_str());
					if(prop->value != NULL){
						if(prop->type == Shader::Property::FLOAT){
							propertyXML->SetAttribute("default", *(float*)(prop->value));
						}
					}
					propertiesXML->LinkEndChild(propertyXML);
				}
				shaderXML->LinkEndChild(propertiesXML);
			}
			shadersXML->LinkEndChild(shaderXML);
		}

		sceneXML->LinkEndChild(shadersXML);
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

	if(materials.size() > 0){
		XMLElement* materialsXML = doc.NewElement("Materials");
		for(pair<S32, Material*> pair : materials){
			S32 id = pair.first;
			Material* material = pair.second;
			XMLElement* materialXML = doc.NewElement("Material");
			materialXML->SetAttribute("id", id);
			S32 shaderID = FindShaderID(material->GetShader());
			materialXML->SetAttribute("shader", shaderID);

			for(Shader::Property* prop : material->properties){
				XMLElement* propertyXML = doc.NewElement("Property");
				switch(prop->type) {
				case Shader::Property::SAMPLER:{
					propertyXML->SetAttribute("type", "Texture");
					propertyXML->SetAttribute("name", prop->name.c_str());
					Texture* usedTexture = gfx2D->FindTextureByGLID(*(GLuint*)prop->value);
					S32 textureId = FindTextureID(usedTexture);
					propertyXML->SetAttribute("value", textureId);}
					break;
				case Shader::Property::MAT4:
					throw CrossException("Property don't supported by loader version %d", scene_loader_version);
					break;
				case Shader::Property::VEC4:
					throw CrossException("Property don't supported by loader version %d", scene_loader_version);
					break;
				case Shader::Property::VEC3:
					throw CrossException("Property don't supported by loader version %d", scene_loader_version);
					break;
				case Shader::Property::VEC2:
					throw CrossException("Property don't supported by loader version %d", scene_loader_version);
					break;
				case Shader::Property::FLOAT:
					propertyXML->SetAttribute("type", "Float");
					propertyXML->SetAttribute("name", prop->name.c_str());
					propertyXML->SetAttribute("value", *(float*)prop->value);
					break;
				case Shader::Property::INT:
					throw CrossException("Property don't supported by loader version %d", scene_loader_version);
					break;
				case Shader::Property::CUBEMAP:
					throw CrossException("Property don't supported by loader version %d", scene_loader_version);
					break;
				default:
					throw CrossException("Unknown material property");
					break;
				}
				materialXML->LinkEndChild(propertyXML);
			}
			materialsXML->LinkEndChild(materialXML);
		}
		sceneXML->LinkEndChild(materialsXML);
	}

	if(models.size() > 0){
		XMLElement* modelsXML = doc.NewElement("Models");
		for(pair<S32, Model*> pair : models){
			XMLElement* modelXML = doc.NewElement("Model");
			S32 id = pair.first;
			Model* model = pair.second;
			modelXML->SetAttribute("id", id);
			modelXML->SetAttribute("file", model->filename.c_str());
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
	File gameConfig;
	gameConfig.name = filename;
	gameConfig.size = printer.CStrSize();
	gameConfig.data = (Byte*)printer.CStr();
	sys->SaveFile(&gameConfig);
	gameConfig.data = NULL;
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
	throw CrossException("Can not find entity %s", name.c_str());
}

void Scene::AddEntity(Entity* entity){
	entity->Initialize();
	root->AddChild(entity);
	EntityAdded(entity);//trigger
}

void Scene::AddModel(Model* model){
	models[model_id] = model;
	model_id++;
	AddEntity(model->hierarchy);
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

void Scene::SetAmbientColor(const Color& color){
	this->ambient_color = color;
}

S32 Scene::FindShaderID(Shader* shader){
	for(pair<S32, Shader*> pair : shaders){
		if(pair.second == shader){
			return pair.first;
		}
	}
	return -1;
}

S32 Scene::FindTextureID(Texture* texture){
	for(pair<S32, Texture*> pair : textures) {
		if(pair.second->name == texture->name) {
			return pair.first;
		}
	}
	return -1;
}

void Scene::WindowResizeHandle(S32 width, S32 height){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, sys->GetAspectRatio(), 0.1f, config->GetViewDistance());
	camera->SetProjectionMatrix(projection);
}

void Scene::LoadEntity(Entity* parent, XMLElement* objectXML){
	const char* name = objectXML->Attribute("name");
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
	if(rotXML){
		double x = rotXML->DoubleAttribute("x");
		double y = rotXML->DoubleAttribute("y");
		double z = rotXML->DoubleAttribute("z");
		double angle = rotXML->DoubleAttribute("angle");
		Quaternion rot(Vector3D((float)x, (float)y, (float)z), angle);
		entity->SetRotate(rot);
	}
	XMLElement* scaleXML = objectXML->FirstChildElement("Scale");
	if(scaleXML){
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
			S32 modelID = meshXML->IntAttribute("modelID");
			Model* model = models[modelID];
			Mesh* mesh = model->meshes[id]->Clone();
			mesh->SetMaterial(gfx3D->GetDefaultMaterial());
			entity->AddComponent(mesh);
		}
	}

	XMLElement* childrenXML = objectXML->FirstChildElement("Children");
	if(childrenXML){
		XMLElement* childXML = childrenXML->FirstChildElement("Object");
		while(childXML){
			LoadEntity(entity, childXML);
			childXML = childXML->NextSiblingElement("Object");
		}
	}
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

	Mesh* mesh = (Mesh*)entity->GetComponent(Component::MESH);
	if(mesh){
		XMLElement* componentsXML = doc->NewElement("Components");
		pair<S32, S32> ids = GetModelMeshID(mesh);
		XMLElement* meshXML = doc->NewElement("Mesh");
		meshXML->SetAttribute("id", ids.second);
		meshXML->SetAttribute("modelID", ids.first);
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

pair<S32, S32> Scene::GetModelMeshID(Mesh* mesh){
	for(pair<S32, Model*> p : models){
		S32 id = p.first;
		Model* model = p.second;
		for(pair<S32, Mesh*> p2 : model->meshes){
			if(p2.second == mesh){
				return pair<S32, S32>(id, p2.first);
			}
		}
	}
	throw CrossException("Can not find mesh ids");
}