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
#include "TransparencyScene.h"
#include "GraphicsGL.h"
#include "Graphics3D.h"
#include "Entity.h"
#include "Light.h"
#include "Shaders/LightsShader.h"
#include "Game.h"
#include "Material.h"
#include "Texture.h"

void TransparencyScene::Start(){
	CameraControlsScene::Start();

	SetAmbientColor(Color(0.15f));

	//lights
	Light* lightComponent = new Light(Light::Type::POINT);
	lightComponent->SetIntensity(0.01f);
	light = new Entity();
	light->AddComponent(lightComponent);
	AddEntity(light);

	road_shader = new LightsShader();
	road_shader->AddProperty("Transparency", "uTransparency", 1.f);
	road_shader->AddMacro("USE_DIFFUSE_MAP");
	road_shader->AddMacro("USE_TILLING_FACTOR");
	road_shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	road_shader->AddProperty("Tilling Factor", "uTillingFactor", 1.f);
	road_shader->AddProperty("Specular", "uSpecular", 0.5f);
	road_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	road_shader->Compile();
	
	road_diffuse = GetTexture("gfx3D/RoadDiffuse.png");
	road_diffuse->SetTilingMode(Texture::TilingMode::REPEAT);
	road_mat = new Material(road_shader);
	road_mat->SetPropertyValue("Diffuse Texture", road_diffuse);
	road_mat->SetPropertyValue("Tilling Factor", 3.f);
	Entity* road = LoadPrimitive(Graphics3D::Primitives::PLANE);
	road->SetScale(15.f);
	gfx3D->AdjustMaterial(road, road_mat, false);
	AddEntity(road);

	grass_shader = new LightsShader();
	grass_shader->AddProperty("Transparency", "uTransparency", 1.f);
	grass_shader->AddMacro("USE_DIFFUSE_MAP");
	grass_shader->AddMacro("USE_CUTOUT");
	grass_shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	grass_shader->AddProperty("Specular", "uSpecular", 0.5f);
	grass_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	grass_shader->Compile();
	grass_diffuse = GetTexture("gfx3D/GrassDiffuse.png");
	Entity* grass = LoadPrimitive(Graphics3D::Primitives::PLANE);
	grass_mat = new Material(grass_shader);
	grass_mat->SetPropertyValue("Diffuse Texture", grass_diffuse);
	grass_mat->SetTransparency(true);
	grass->SetRotateX(90.f);
	AddEntity(grass);

	gfx3D->AdjustMaterial(grass, grass_mat, false);

	for(U32 i = 0; i < 10; ++i){
		Entity* clone = grass->Clone();
		clone->SetRotateX(90.f);
		clone->SetPosition(Vector3D(Random(-5.f, 5.f), .5f, Random(-5.f, 5.f)));
		Quaternion quat(Vector3D::Up, Random(360.f));
		clone->SetRotate(quat * clone->GetRotation());
		AddEntity(clone);
	}

	sphere_shader = new LightsShader();
	sphere_shader->AddProperty("Transparency", "uTransparency", 1.f);
	sphere_shader->AddProperty("Color", "uDiffuseColor", Color::Blue);
	sphere_shader->AddProperty("Specular", "uSpecular", 0.5f);
	sphere_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	sphere_shader->Compile();

	sphere_mat = new Material(sphere_shader);
	sphere_mat->SetPropertyValue("Transparency", 0.5f);
	sphere_mat->SetTransparency(true);

	Entity* sphere = LoadPrimitive(Graphics3D::Primitives::SPHERE);
	gfx3D->AdjustMaterial(sphere, sphere_mat, true);
	AddEntity(sphere);
	sphere->SetPosition(Vector3D(0.f, 1.f, 0.f));
}

void TransparencyScene::Stop(){
	delete sphere_mat;
	delete sphere_shader;
	delete grass_mat;
	delete grass_diffuse;
	delete grass_shader;
	delete road_mat;
	delete road_diffuse;
	delete road_shader;
	CameraControlsScene::Stop();
}

void TransparencyScene::Update(float sec){
	CameraControlsScene::Update(sec);
	light->SetPosition(Vector3D(cos(game->GetRunTime() / 2.f)*3.f, 2.f, sin(game->GetRunTime() / 2.f)*3.f));
}