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
#include "Graphics2D.h"
#include "Graphics3D/Shaders/LightShader.h"
#include "Material.h"
#include "Mesh.h"
#include "Utils/Light.h"

void DirectionalLightScene::Start(){
	CCScene::Start();

	SetOrbitDistance(60.f);

	Light* light = new Light(Light::DIRECTIONAL);
	light->SetDirection(Vector3D(0.f, 0.f, 1.f));
	AddLight(light);

	shader = new LightShader("gfx3D/shaders/directional_light.vert", "gfx3D/shaders/directional_light.frag");
	material = new Material(shader);
	Texture* diffuseTexture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse.png", Texture::Filter::TRILINEAR);
	Texture* specularTexture = gfx2D->LoadTexture("gfx3D/ContainerSpecular.png", Texture::Filter::TRILINEAR);
	material->SetDiffuseTexture(diffuseTexture);
	material->SetSpecularTexture(specularTexture);
	cube = gfx3D->LoadMesh("gfx3D/Cube.obj");
	cube->SetMaterial(material);

	for(int i = 0; i < 10; ++i){
		Mesh* clone = cube->Clone();
		clone->SetPosition(Vector3D(Random(-20.f, 20.f), Random(-20.f, 20.f), Random(-20.f, 20.f)));
		clone->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		objects.push_back(clone);
	}
}

void DirectionalLightScene::Stop(){
	for(Mesh* clone : objects){
		delete clone;
	}
	delete cube;
	delete material;
	delete shader;
	CCScene::Stop();
}

void DirectionalLightScene::Update(float sec){
	for(Mesh* obj : objects){
		obj->Draw();
	}
	CCScene::Update(sec);
}