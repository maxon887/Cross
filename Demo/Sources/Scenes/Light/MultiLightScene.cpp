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
#include "MultiLightScene.h"
#include "Light.h"
#include "Material.h"
#include "Entity.h"
#include "Camera.h"
#include "Shaders/LightsShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"

void MultiLightScene::Start(){
	DemoScene::Start();

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -30.f));

	for(U32 i = 0; i < 8; ++i){
		Entity* light = new Entity("PointLight_" + String(i));
		light->AddComponent(new Transform());
		Light* lightComponent = new Light(Light::Type::POINT);
		light->GetTransform()->SetPosition(Vector3D(Random(-10.f, 10.f), Random(-10.f, 10.f), Random(-10.f, 10.f)));
		lightComponent->SetColor(Color::Red);
		light->AddComponent(lightComponent);
		AddEntity(light);
	}

	for(U32 i = 0; i < 1; ++i){
		Entity* light = new Entity("DirectionalLight_" + String(i));
		light->AddComponent(new Transform());
		Light* lightComponent = new Light(Light::Type::DIRECTIONAL);
		light->GetTransform()->SetDirection(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)));
		lightComponent->SetColor(Color::Green);
		light->AddComponent(lightComponent);
		AddEntity(light);
	}

	for(U32 i = 0; i < 4; ++i){
		Entity* light = new Entity("SpotLight_" + String(i));
		light->AddComponent(new Transform());
		Light* lightComponent = new Light(Light::Type::SPOT);
		light->GetTransform()->SetPosition(Vector3D(Random(-10.f, 10.f), Random(-10.f, 10.f), Random(-10.f, 10.f)));
		light->GetTransform()->LookAt(Vector3D(0.f));
		lightComponent->SetColor(Color::Blue);
		light->AddComponent(lightComponent);
		AddEntity(light);
	}

	shader = new LightsShader();
	shader->AddProperty("Transparency", "uTransparency", 1.f);
	shader->AddMacro("USE_DIFFUSE_MAP");
	shader->AddMacro("USE_SPECULAR_MAP");
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Specular Map", "uSpecularMap");
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();

	material = new Material(shader);
	material->SetPropertyValue("Diffuse Texture", GetTexture("Textures/ContainerDiffuse.png"));
	material->SetPropertyValue("Specular Map", GetTexture("Textures/ContainerSpecular.png"));
	material->SetPropertyValue("Shininess", 0.5f * 128.f);
	cube = LoadPrimitive(Model::Primitive::CUBE);
	cube->GetComponent<Mesh>()->SetMaterial(material);
	Entity* cubes = new Entity("Cubes");
	cubes->AddComponent(new Transform());
	AddEntity(cubes);
	for(U32 i = 0; i < 50; ++i){
		Entity* clone = cube->Clone();
		clone->GetTransform()->SetPosition(Vector3D(Random(-10.f, 10.f), Random(-10.f, 10.f), Random(-10.f, 10.f)));
		clone->GetTransform()->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		cubes->AddChild(clone);
	}
}

void MultiLightScene::Stop(){
	delete material;
	delete shader;
	delete cube;
	DemoScene::Stop();
}