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
#include "PhysicsScreen.h"
#include "Entity.h"
#include "GraphicsGL.h"
#include "Graphics3D.h"
#include "Shaders/LightShader.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"
#include "Shaders/MultiLightShader.h"
#include "Texture.h"
#include "Graphics2D.h"
#include "Physics/RigidBody.h"

void PhysicsScreen::Start() {
	CameraControlsScene::Start();
	LookAtCamera(false);
	GetCamera()->SetPosition(Vector3D(0.f, 3.f, -4.f));
	GetCamera()->SetDirection(Vector3D(0.8f, 0.2f, 0.5f));
	//light setups
	Entity* light = new Entity();
	light->AddComponent(new Light(Light::Type::POINT));
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddEntity(light);
	SetBackground(Color(0.3f));
	//*********************ROAD**********************
	road_shader = (MultiLightShader*)gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	road_shader->AddMakro("USE_DIFFUSE_MAP");
	road_shader->AddMakro("USE_TILLING_FACTOR");
	road_shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	road_shader->AddProperty("Specular", "uSpecular", 0.5f);
	road_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	road_shader->AddProperty("Tilling Factor", "uTillingFactor", 12.f);
	road_shader->Compile();
	road_diffuse = gfx2D->LoadTexture("gfx3D/RoadDiffuse.png");
	road_diffuse->SetTilingMode(Texture::TilingMode::REPEAT);
	road_mat = new Material(road_shader);
	road_mat->SetPropertyValue("Diffuse Texture", road_diffuse);

	Entity* road = gfx3D->LoadPrimitive(Graphics3D::Primitives::PLANE);
	road->SetScale(250.f);
	gfx3D->AdjustMaterial(road, road_mat, false);
	AddEntity(road);
	//*********************BALLS SHADER**********************
	ball_shader = new LightShader("gfx3D/shaders/specular.vert", "gfx3D/shaders/specular.frag");
	ball_shader->AddProperty("Diffuse Color", "uColor");
	ball_shader->AddProperty("Specular Color", "uSpecularColor");
	ball_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	ball_shader->Compile();
	//*********************RED BALL**********************
	red_mat = new Material(ball_shader);
	red_mat->SetPropertyValue("Diffuse Color", Color::Red);
	red_mat->SetPropertyValue("Specular Color", Color::White);


	Entity* redBall = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	gfx3D->AdjustMaterial(redBall, red_mat);
	redBall->SetPosition(Vector3D(-1.f, 1.f, 0.f));
	RigidBody* rigid = new RigidBody();
	rigid->SetVelocity(Vector3D(25.f, 25.f, 0.f));
	redBall->AddComponent(rigid);
	AddEntity(redBall);
	//*********************GREEN BALL**********************
	green_mat = new Material(ball_shader);
	green_mat->SetPropertyValue("Diffuse Color", Color::Green);
	green_mat->SetPropertyValue("Specular Color", Color::White);

	Entity* greenBall = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	gfx3D->AdjustMaterial(greenBall, green_mat);
	greenBall->SetPosition(Vector3D(2.f, 1.f, 0.f));
	AddEntity(greenBall);
}

void PhysicsScreen::Stop() {
	delete green_mat;
	delete red_mat;
	delete ball_shader;
	delete road_mat;
	delete road_diffuse;
	delete road_shader;
	CameraControlsScene::Stop();
}

void PhysicsScreen::Update(float sec) {
	CameraControlsScene::Update(sec);
	//GetCamera()->LookAt(redBall->GetPosition());
}