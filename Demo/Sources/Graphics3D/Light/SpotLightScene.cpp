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
#include "SpotLightScene.h"
#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Shaders/SingleLightShader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
	
void SpotLightScene::Start(){
	DemoScene::Start();

	GetCamera()->SetPosition(Vector3D(-8.f, 0.f, -15.f));
    GetCamera()->GetTransform()->LookAt(Vector3D::Zero);

	Entity* light = new Entity("SpotLight");
	light->AddComponent(new Transform());
	Light* lightComponent = new Light(Light::Type::SPOT);
	light->GetTransform()->SetPosition(Vector3D(0.f, 0.f, -8.f));
	light->GetTransform()->LookAt(Vector3D(0.f));
	lightComponent->SetCutOff(20.f);
	light->AddComponent(lightComponent);
	AddEntity(light);

	shader = new SingleLightShader("gfx3D/shaders/spot_light.vert", "gfx3D/shaders/spot_light.frag");
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Specular Map", "uSpecularMap");
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();
	material = new Material(shader);
	material->SetPropertyValue("Diffuse Texture", GetTexture("gfx3D/ContainerDiffuse.png"));
	material->SetPropertyValue("Specular Map", GetTexture("gfx3D/ContainerSpecular.png"));
	material->SetPropertyValue("Shininess", 0.5f * 128.f);
	cube = LoadPrimitive(Model::Primitive::CUBE);
	cube->GetComponent<Mesh>()->SetMaterial(material);

	for(U32 i = 0; i < 40; ++i){
		Entity* clone = cube->Clone();
		clone->GetTransform()->SetPosition(Vector3D(Random(-5.f, 5.f), Random(-5.f, 5.f), Random(-5.f, 5.f)));
		clone->GetTransform()->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		AddEntity(clone);
	}
}

void SpotLightScene::Stop(){
	delete material;
	delete shader;
	delete cube;
	DemoScene::Stop();
}

void SpotLightScene::Update(float sec){
	DemoScene::Update(sec);
}
