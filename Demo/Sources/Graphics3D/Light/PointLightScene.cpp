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
#include "PointLightScene.h"
#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Shaders/SingleLightShader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
	
void PointLightScene::Start(){
	DemoScene::Start();

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -8.f));

	Entity* light = new Entity();
	light->AddComponent(new Light(Light::Type::POINT));
	AddEntity(light);

	shader = new SingleLightShader("gfx3D/shaders/point_light.vert", "gfx3D/shaders/point_light.frag");
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Specular Map", "uSpecularMap");
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();
	material = new Material(shader);
	diffuse_texture = GetTexture("gfx3D/ContainerDiffuse.png");
	specular_map = GetTexture("gfx3D/ContainerSpecular.png");
	material->SetPropertyValue("Diffuse Texture", diffuse_texture);
	material->SetPropertyValue("Specular Map", specular_map);
	material->SetPropertyValue("Shininess", 0.5f * 128.f);
	cube = LoadPrimitive(Model::Primitive::CUBE);
	cube->GetComponent<Mesh>()->SetMaterial(material);

	for(U32 i = 0; i < 50; ++i){
		Entity* clone = cube->Clone();
		clone->SetPosition(Vector3D(Random(-15.f, 15.f), Random(-15.f, 15.f), Random(-15.f, 15.f)));
		clone->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		AddEntity(clone);
	}
}

void PointLightScene::Stop(){
	delete material;
	delete specular_map;
	delete diffuse_texture;
	delete shader;
	delete cube;
	DemoScene::Stop();
}

void PointLightScene::Update(float sec){
	DemoScene::Update(sec);
}