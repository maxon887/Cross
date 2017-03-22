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
#include "Physics/AABoxCollider.h"
#include "Entity.h"
#include "GraphicsGL.h"
#include "Graphics3D.h"
#include "Shaders/LightShader.h"
#include "Material.h"
#include "Model.h"
#include "Light.h"
#include "Physics/RigidBody.h"
#include "Physics/SphereCollider.h"
#include "Camera.h"
#include "Shaders/MultiLightShader.h"
#include "Texture.h"
#include "Graphics2D.h"

void PhysicsScreen::Start() {
	CameraControlsScene::Start();
	GetCamera()->SetPosition(Vector3D(0.f, 3.f, -4.f));
	GetCamera()->LookAt(Vector3D(0.f));
	//light setups
	Light* light = new Light(Light::Type::POINT);
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddLight(light);
	//*********************ROAD**********************
	MultiLightShader* road_shader = (MultiLightShader*)gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	road_shader->AddMakro("USE_DIFFUSE_MAP");
	road_shader->AddMakro("USE_TILLING_FACTOR");
	road_shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	road_shader->AddProperty("Specular", "uSpecular");
	road_shader->AddProperty("Shininess", "uShininess");
	road_shader->AddProperty("Tilling Factor", "uTillingFactor");
	road_shader->Compile();
	Texture* road_diffuse = gfx2D->LoadTexture("gfx3D/RoadDiffuse.png");
	road_diffuse->SetTilingMode(Texture::TilingMode::REPEAT);
	Material* road_mat = new Material(road_shader);
	road_mat->SetPropertyValue("Diffuse Texture", road_diffuse);
	road_mat->SetPropertyValue("Specular", 0.5f);
	road_mat->SetPropertyValue("Shininess", 0.5f * 128.f);
	road_mat->SetPropertyValue("Tilling Factor", 12.f);

	Entity* road = gfx3D->LoadPrimitive(Graphics3D::Primitives::PLANE);
	road->SetScale(250.f);
	gfx3D->AdjustMaterial(road, road_mat, false);
	AddEntity(road);
	//*********************RED BALL**********************
	//Model component
	LightShader* shader = new LightShader("gfx3D/shaders/specular.vert", "gfx3D/shaders/specular.frag");
	shader->AddProperty("Diffuse Color", "uColor");
	shader->AddProperty("Specular Color", "uSpecularColor");
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();
	Material* redMaterial = new Material(shader);
	redMaterial->SetPropertyValue("Diffuse Color", Color::Red);
	redMaterial->SetPropertyValue("Specular Color", Color::White);
	redMaterial->SetPropertyValue("Shininess", 0.5f * 128.f);
	//Model* redSphere = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	//redSphere->SetMaterial(redMaterial);
	//Collider component
	SphereCollider* redCollider = new SphereCollider(0.5f);
	//Body component
	RigidBody* redBody = new RigidBody();
	redBody->AddVelocity(Vector3D(3.f, 0.f, -0.5f));
	redBody->UseGravity(false);

	redBall = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	gfx3D->AdjustMaterial(redBall, redMaterial);
	redBall->SetPosition(Vector3D(-1.f, 1.f, 0.f));
	redBall->AddComponent(redBody);
	redBall->AddComponent(redCollider);
	AddEntity(redBall);
	//*********************GREEN BALL**********************
	Material* greenMaterial = new Material(shader);
	greenMaterial->SetPropertyValue("Diffuse Color", Color::Green);
	greenMaterial->SetPropertyValue("Specular Color", Color::White);
	greenMaterial->SetPropertyValue("Shininess", 0.5f * 128.f);

	//Model* greenSphere = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	//greenSphere->SetMaterial(greenMaterial);
	//Collider component
	SphereCollider* greenCollider = new SphereCollider(0.5f);
	//RigidBody component
	//RigidBody* greenBody = new RigidBody();
	//greenBody->AddVelocity(Vector3D(-0.6f, 0.f, 0.2f));
	//greenBody->UseGravity(false);

	Entity* greenBall = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	gfx3D->AdjustMaterial(greenBall, greenMaterial);
	greenBall->SetPosition(Vector3D(2.f, 1.f, 0.f));
	//greenBall->AddComponent(greenBody);
	greenBall->AddComponent(greenCollider);

	AddEntity(greenBall);
}

void PhysicsScreen::Stop() {

	CameraControlsScene::Stop();
}

void PhysicsScreen::Update(float sec) {
	CameraControlsScene::Update(sec);
	//GetCamera()->LookAt(redBall->GetPosition());
}