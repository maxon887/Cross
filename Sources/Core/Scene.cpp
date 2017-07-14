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
	for(pair<U32, Shader*> pair : shaders){
		delete pair.second;
	}
	for(pair<U32, Texture*> pair : textures){
		delete pair.second;
	}
	for(pair<U32, Material*> pair : materials){
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

void Scene::Load(const string& file, bool absolute){
	Clear();
	string path = "";
	if(!absolute){
		path = sys->AssetsPath() + file;
	}else{
		path = file;
	}

	XMLDocument doc;
	XMLError error = doc.LoadFile(path.c_str());
	if(error != XML_SUCCESS){
		throw CrossException("Can not parse XML document");
	}

	XMLElement* scene = doc.FirstChildElement("Scene");
	if(scene){
		name = scene->Attribute("name");
		SetName(name);
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
		//objects loading
		XMLElement* objectsXML = scene->FirstChildElement("Objects");
		if(objectsXML){
			XMLElement* objectXML = objectsXML->FirstChildElement("Object");
			while(objectXML){
				const char* name = objectXML->Attribute("name");
				const char* file = objectXML->Attribute("file");
				Entity* entity = NULL;
				if(file){
					int materialID = objectXML->IntAttribute("material");

					entity = gfx3D->LoadModel(file);
					if(materialID != -1){
						gfx3D->AdjustMaterial(entity, materials[materialID]);
					}else{
						gfx3D->AdjustMaterial(entity, gfx3D->GetDefaultMaterial()->Clone());
					}
				}else{
					entity = new Entity();
				}
				entity->SetName(name);

				XMLElement* posXML = objectXML->FirstChildElement("Position");
				if(posXML){
					double x = posXML->DoubleAttribute("x");
					double y = posXML->DoubleAttribute("y");
					double z = posXML->DoubleAttribute("z");
					Vector3D pos((float)x, (float)y, (float)z);
					entity->SetPosition(pos);
				}

				XMLElement* componentsXML = objectXML->FirstChildElement("Components");
				if(componentsXML){
					XMLElement* componentXML = componentsXML->FirstChildElement("Component");
					while(componentXML){
						const char* name = componentXML->Attribute("name");
						if(strcmp(name, "Light") == 0){
							const char* type = componentXML->Attribute("type");
							if(strcmp(type, "Point") == 0){
								Light* light = new Light(Light::Type::POINT);
								entity->AddComponent(light);
							}else{
								throw CrossException("Unknown light type");
							}
						}else{
							throw CrossException("Unknown component");
						}
						componentXML = componentXML->NextSiblingElement();
					}
				}
				AddEntity(entity);
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
		for(int i = 0; i < shaders.size(); i++){
			XMLElement* shaderXML= doc.NewElement("Shader");
			shaderXML->SetAttribute("id", i);
			if(dynamic_cast<MultiLightShader*>(shaders[i])){
				shaderXML->SetAttribute("multiLight", true);
			}
			if(shaders[i]->macrosies.size() > 0){
				XMLElement* macrosiesXML = doc.NewElement("Macrosies");
				for(const string& macro : shaders[i]->user_macro){
					XMLElement* macroXML = doc.NewElement("Macro");
					macroXML->SetText(macro.c_str());
					macrosiesXML->LinkEndChild(macroXML);
				}
				shaderXML->LinkEndChild(macrosiesXML);
				XMLElement* propertiesXML = doc.NewElement("Properties");
				for(Shader::Property* prop : shaders[i]->properties){
					XMLElement* properyXML = doc.NewElement("Property");
					propertiesXML->SetAttribute("name", prop->name.c_str());
					propertiesXML->SetAttribute("glName", prop->glName.c_str());
					if(prop->value != NULL){
						if(prop->type == Shader::Property::FLOAT){
							propertiesXML->SetAttribute("default", *(float*)(prop->value));
						}
					}
				}
			}
			shadersXML->LinkEndChild(shaderXML);
		}

		sceneXML->LinkEndChild(shadersXML);
	}

	if(textures.size() > 0){
		XMLElement* texturesXML = doc.NewElement("Textures");

		sceneXML->LinkEndChild(texturesXML);
	}

	if(materials.size() > 0){
		XMLElement* materialsXML = doc.NewElement("Materials");

		sceneXML->LinkEndChild(materialsXML);
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
	for(pair<U32, Material*> pair : materials){
		delete pair.second;
	}
	materials.clear();
	for(pair<U32, Texture*> pair : textures) {
		delete pair.second;
	}
	textures.clear();
	for(pair<U32, Shader*> pair : shaders) {
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

void Scene::WindowResizeHandle(S32 width, S32 height){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, sys->GetAspectRatio(), 0.1f, config->GetViewDistance());
	camera->SetProjectionMatrix(projection);
}