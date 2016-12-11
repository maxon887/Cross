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
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Model.h"
#include "Light.h"
#include "Shaders/MultiLightShader.h"
#include "Game.h"
#include "Material.h"
#include "Texture.h"

#include <math.h>

void TransparencyScene::Start(){
	CameraControlsScreen::Start();

	SetAmbientColor(Color(0.15f));

	//lights
	light = new Light(Light::Type::POINT);
	light->SetIntensity(0.01f);
	AddLight(light);

	road_shader = (MultiLightShader*)gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	road_shader->AddMakro("USE_DIFFUSE_MAP");
	road_shader->AddMakro("USE_TILLING_FACTOR");
	road_shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	road_shader->AddProperty("Tilling Factor", "uTillingFactor", 1.f);
	road_shader->AddProperty("Specular", "uSpecular", 0.5f);
	road_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	road_shader->Compile();
	
	road_diffuse = gfx2D->LoadTexture("gfx3D/RoadDiffuse.png");
	road_diffuse->SetTilingMode(Texture::TilingMode::REPEAT);
	road_mat = new Material(road_shader);
	road_mat->SetPropertyValue("Diffuse Texture", road_diffuse);
	road_mat->SetPropertyValue("Tilling Factor", 3.f);
	road = gfx3D->LoadPrimitive(Graphics3D::Primitives::PLANE);
	road->SetScale(15.f);
	road->FaceCulling(false);
	road->SetMaterial(road_mat);

	grass_shader = (MultiLightShader*)gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	grass_shader->AddMakro("USE_DIFFUSE_MAP");
	grass_shader->AddMakro("USE_CUTOUT");
	grass_shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	grass_shader->AddProperty("Specular", "uSpecular", 0.5f);
	grass_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	grass_shader->Compile();
	grass_diffuse = gfx2D->LoadTexture("gfx3D/GrassDiffuse.png");
	grass = gfx3D->LoadPrimitive(Graphics3D::Primitives::PLANE);
	grass_mat = new Material(grass_shader);
	grass_mat->SetPropertyValue("Diffuse Texture", grass_diffuse);
	grass->SetRotateX(90.f);
	grass->FaceCulling(false);
	grass->SetMaterial(grass_mat);

	for(U32 i = 0; i < 10; ++i){
		Model* clone = grass->Clone();
		clone->SetPosition(Vector3D(Random(-5.f, 5.f), .5f, Random(-5.f, 5.f)));
		Quaternion quat(Vector3D::Up, Random(360.f));
		clone->SetRotate(quat * clone->GetRotation());
		models.push_back(clone);
	}

	sphere_shader = (MultiLightShader*)gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	sphere_shader->AddProperty("Color", "uDiffuseColor", Color::Blue);
	sphere_shader->AddProperty("Specular", "uSpecular", 0.5f);
	sphere_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	sphere_shader->Compile();

	sphere_mat = new Material(sphere_shader);
	sphere_mat->SetPropertyValue("Transparency", 0.5f);

	sphere = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	sphere->SetMaterial(sphere_mat);
	sphere->AlphaBlending(true);
	sphere->SetPosition(Vector3D(0.f, 1.f, 0.f));
}

void TransparencyScene::Stop(){
	delete road;
	delete road_mat;
	delete grass_mat;
	delete road_diffuse;
	delete grass_diffuse;
	delete road_shader;
	CameraControlsScreen::Stop();
}

void TransparencyScene::Update(float sec){
	CameraControlsScreen::Update(sec);
	road->Draw();
	sphere->Draw();
	light->SetPosition(Vector3D(cos(game->GetRunTime() / 2.f)*3.f, 2.f, sin(game->GetRunTime() / 2.f)*3.f));
}