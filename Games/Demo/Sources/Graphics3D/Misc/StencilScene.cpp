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
#include "StencilScene.h"
#include "Graphics3D.h"
#include "Material.h"
#include "Model.h"
#include "Light.h"
#include "Shaders/LightShader.h"
#include "Graphics2D.h"
#include "Config.h"
#include "Camera.h"

void StencilScene::Start(){
	CameraControlsScreen::Start();

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -6.f));
	//light setups
	Light* light = new Light(Light::POINT);
	light->SetPosition(Vector3D(2.f, 2.f, -2.f));
	AddLight(light);
	//scene setups
	shader = gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();
	cube_texture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse.png");
	cube_mat = new Material(shader);
	cube_mat->SetPropertyValue("Diffuse Texture", cube_texture);
	cube_mat->SetPropertyValue("Shininess", 0.5f * 128.f);
	shader_outline = gfxGL->GetShader(DefaultShader::SIMPLE);
	shader_outline->Compile();
	cube_outline = new Material(shader_outline);
	cube_outline->SetPropertyValue("Color", Color::Blue);
	cube = gfx3D->LoadPrimitive(Graphics3D::Primitives::CUBE);
	cube->SetPosition(Vector3D(0.f, 0.5f, 0.f));
	road_texture = gfx2D->LoadTexture("gfx3D/RoadDiffuse.png");
	road_texture->SetTilingMode(Texture::TilingMode::REPEAT);
	road_mat = new Material(shader);
	road_mat->SetPropertyValue("Diffuse Texture", road_texture);
	road_mat->SetPropertyValue("Shininess", 0.5f * 128.f);
	road_mat->SetPropertyValue("Tilling Factor", 3.f);
	road = gfx3D->LoadPrimitive(Graphics3D::Primitives::PLANE);
	road->SetScale(25.f);
	road->SetMaterial(road_mat);
	road->FaceCulling(false);

	for(U32 i = 0; i < 10; ++i){
		Model* clone = cube->Clone();
		clone->SetPosition(Vector3D(Random(-10.f, 10.f), 0.5f, Random(-10.f, 10.f)));
		clone->SetRotate(Vector3D::Up, Random(0.f, 360.f));
		models.push_back(clone);
	}
}

void StencilScene::Stop(){
	for(Model* model : models){
		delete model;
	}
	delete cube;
	delete road;
	delete cube_mat;
	delete cube_outline;
	delete road_mat;
	delete cube_texture;
	delete road_texture;
	delete shader_outline;
	delete shader;
	CameraControlsScreen::Stop();
}

void StencilScene::Update(float sec){
	CameraControlsScreen::Update(sec);
	for(Model* model : models){
		model->SetScale(1.f);
		model->SetStencil(Graphics3D::StencilBehaviour::WRITE);
		model->SetMaterial(cube_mat);
		model->Draw();
		model->SetScale(1.1f);
		model->SetStencil(Graphics3D::StencilBehaviour::READ);
		model->SetMaterial(cube_outline);
		model->Draw();
	}
	road->Draw();
}