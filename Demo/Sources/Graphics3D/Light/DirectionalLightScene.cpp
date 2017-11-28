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
#include "DirectionalLightScene.h"
#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Shaders/SingleLightShader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Transform.h"

void DirectionalLightScene::Start(){
	DemoScene::Start();

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -8.f));

	Entity* light = new Entity("DirectionalLight");
	light->AddComponent(new Transform(Vector3D(0.f, 0.f, 1.f)));
	light->AddComponent(new Light(Light::Type::POINT));
	AddEntity(light);

	shader = new SingleLightShader("gfx3D/shaders/directional_light.vert", "gfx3D/shaders/directional_light.frag");
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
	Entity* cubes = new Entity("Cubes");
	cubes->AddComponent(new Transform());
	AddEntity(cubes);
	for(U32 i = 0; i < 10; ++i){
		Entity* clone = cube->Clone();
		clone->GetTransform()->SetPosition(Vector3D(Random(-3.f, 3.f), Random(-3.f, 3.f), Random(-3.f, 3.f)));
		clone->GetTransform()->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		cubes->AddChild(clone);
	}
}

void DirectionalLightScene::Stop(){
	delete cube;
	delete material;
	delete shader;
	DemoScene::Stop();
}