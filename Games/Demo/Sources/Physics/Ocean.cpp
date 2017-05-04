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
#include "Ocean.h"
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
#include "System.h"
#include "Physics/RigidBody.h"

Ocean::BuoyantEntity::BuoyantEntity(Entity* e) : 
	Entity(*e)
{ 
	e->DeleteChildren();
	delete e;
}

void Ocean::BuoyantEntity::Update(float sec){
	if(GetPosition().y <= 0.f){
		RigidBody* rb = (RigidBody*)GetComponent(Component::Type::RIGIDBODY);
		if(rb){
			rb->ApplyForce(Vector3D(0.f, 20.f, 0.f));
		}
	}
	Entity::Update(sec);
}

Ocean::Spring::Spring(RigidBody* obj1, RigidBody* obj2, float restLength) :
	p1(obj1),
	p2(obj2),
	rest_length(restLength),
	fixed(false)
{ }

Ocean::Spring::Spring(Vector3D ancor, RigidBody* obj, float restLength) :
	ancor(ancor),
	p1(obj),
	rest_length(restLength),
	fixed(true)
{ }

void Ocean::Spring::Update(){
	if(fixed){
		Vector3D dir = p1->GetPosition() - ancor;
		float dL = dir.Length() - rest_length;

		p1->ApplyForce(dir.GetNormalized() * -dL * coef);
		gfx3D->DrawLine(p1->GetPosition(), ancor, Color::Red);
	}else{
		Vector3D dir = p1->GetPosition() - p2->GetPosition();
		float dL = dir.Length() - rest_length;

		p1->ApplyForce(dir.GetNormalized() * -dL * coef);
		p2->ApplyForce(dir.GetNormalized() * dL * coef);
		gfx3D->DrawLine(p1->GetPosition(), p2->GetPosition(), Color::Red);
	}
}

void Ocean::Start() {
	CameraControlsScene::Start();
	SetBackground(Color(0.3f));
	LookAtCamera(false);
	GetCamera()->SetPosition(Vector3D(0.f, 3.f, -4.f));
	GetCamera()->SetDirection(Vector3D(0.8f, 0.2f, 0.5f));
	//light setups
	Entity* light = new Entity();
	light->AddComponent(new Light(Light::Type::POINT));
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddEntity(light);
	
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

	
	BuoyantEntity* redBall = new BuoyantEntity(gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE));
	gfx3D->AdjustMaterial(redBall, red_mat);
	redBall->SetPosition(Vector3D(-1.f, 5.f, 0.f));
	RigidBody* rigid = new RigidBody();
	rigid->SetVelocity(Vector3D(5.f, 5.f, 0.f));
	redBall->AddComponent(rigid);
	AddEntity(redBall);
	//*********************GREEN BALL**********************
	green_mat = new Material(ball_shader);
	green_mat->SetPropertyValue("Diffuse Color", Color::Green);
	green_mat->SetPropertyValue("Specular Color", Color::White);

	BuoyantEntity* greenBall =  new BuoyantEntity(gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE));
	gfx3D->AdjustMaterial(greenBall, green_mat);
	greenBall->SetPosition(Vector3D(4.f, 4.f, 0.f));
	greenBall->AddComponent(new RigidBody(1.f));
	AddEntity(greenBall);

	//*********************CONNECTED BALLS**********************
	BuoyantEntity* b1 =  new BuoyantEntity(gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE));
	BuoyantEntity* b2 =  new BuoyantEntity(gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE));
	b1->SetScale(0.5f);
	b2->SetScale(0.5f);
	orange_mat = new Material(ball_shader);
	orange_mat->SetPropertyValue("Diffuse Color", Color(1.f, 0.5f, 0.f));
	orange_mat->SetPropertyValue("Specular Color", Color::White);
	gfx3D->AdjustMaterial(b1, orange_mat);
	gfx3D->AdjustMaterial(b2, orange_mat);
	b1->SetPosition(Vector3D(4.f, 4.f, -2.f));
	b2->SetPosition(Vector3D(3.f, 2.f, 0.f));
	RigidBody* b1rb = new RigidBody();
	b1rb->SetVelocity(Vector3D(0.f, 3.f, 0.f));
	b1->AddComponent(b1rb);
	RigidBody* b2rb = new RigidBody();
	b2rb->SetVelocity(Vector3D(0.f, 3.f, 3.f));
	b2->AddComponent(b2rb);
	free_spring = new Spring(b1rb, b2rb, 3.f);

	AddEntity(b1);
	AddEntity(b2);
	//*********************HOOKED BALL**********************
	BuoyantEntity* hookedBall = new BuoyantEntity(gfx3D->LoadPrimitive(Graphics3D::SPHERE));
	white_mat = new Material(ball_shader);
	white_mat->SetPropertyValue("Diffuse Color", Color::White);
	white_mat->SetPropertyValue("Specular Color", Color::White);
	gfx3D->AdjustMaterial(hookedBall, white_mat);
	hookedBall->SetPosition(Vector3D(7.f, 5.f, 0.f));
	RigidBody* hookedRB = new RigidBody(2.f);
	hookedBall->AddComponent(hookedRB);
	fixed_spring = new Spring(Vector3D(9.f, 8.f, 0.f), hookedRB, 3.5f);
	AddEntity(hookedBall);
	
	//*********************ROAD**********************
	water_shader = (MultiLightShader*)gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	water_shader->AddMakro("USE_TILLING_FACTOR");
	water_shader->AddProperty("Specular", "uSpecular", 0.5f);
	water_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	water_shader->AddProperty("Color", "uDiffuseColor", Color::Blue);
	water_shader->Compile();
	water_mat = new Material(water_shader);
	water_mat->SetPropertyValue("Transparency", 0.65f);
	water_mat->TransparencyEnabled(true);

	Entity* water = gfx3D->LoadPrimitive(Graphics3D::Primitives::PLANE);
	water->SetScale(500.f);
	gfx3D->AdjustMaterial(water, water_mat, false);
	AddEntity(water);
}

void Ocean::Stop() {
	delete fixed_spring;
	delete free_spring;
	delete white_mat;
	delete green_mat;
	delete red_mat;
	delete orange_mat;
	delete ball_shader;
	delete water_mat;
	delete water_shader;
	CameraControlsScene::Stop();
}

void Ocean::Update(float sec) {
	free_spring->Update();
	fixed_spring->Update();
	CameraControlsScene::Update(sec);
	//GetCamera()->LookAt(redBall->GetPosition());
}