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
#include "Shaders/LightsShader.h"
#include "Shaders/SingleLightShader.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"
#include "Texture.h"
#include "System.h"
#include "Physics/RigidBody.h"
#include "Utils/PrimitiveDrawer.h"

/*
void Ocean::BuoyantEntity::AddComponent(Component* newOne) {
	if(newOne->GetType() == Component::Type::RIGIDBODY){
		((RigidBody*)(newOne))->SetDumping(0.2f);
	}
	Entity::AddComponent(newOne);
}*/

void Ocean::Buoyant::Update(float sec) {
	if(GetTransform()->GetPosition().y <= 0.f) {
		RigidBody* rb = GetEntity()->GetComponent<RigidBody>();
		if(rb){
			rb->ApplyForce(Vector3D(0.f, 20.f, 0.f));
		}
	}
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
		PrimitiveDrawer::DrawLine(p1->GetPosition(), ancor, Color::Red);
	}else{
		Vector3D dir = p1->GetPosition() - p2->GetPosition();
		float dL = dir.Length() - rest_length;

		p1->ApplyForce(dir.GetNormalized() * -dL * coef);
		p2->ApplyForce(dir.GetNormalized() * dL * coef);
		PrimitiveDrawer::DrawLine(p1->GetPosition(), p2->GetPosition(), Color::Red);
	}
}

void Ocean::Start() {
	DemoScene::Start();
	SetBackground(Color(0.3f));
	LookAtCamera(false);
	GetCamera()->SetPosition(Vector3D(-2.f, 3.f, -5.f));
	GetCamera()->GetTransform()->SetDirection(Vector3D(0.8f, -0.1f, 0.5f));
	//light setups
	Entity* light = new Entity("Light");
	light->AddComponent(new Transform(Vector3D(10.f, 7.f, -5.f)));
	light->AddComponent(new Light(Light::Type::POINT));
	AddEntity(light);
	
	//*********************BALLS SHADER**********************
	/*
	ball_shader = new LightShader("gfx3D/shaders/specular.vert", "gfx3D/shaders/specular.frag");
	ball_shader->AddProperty("Diffuse Color", "uColor");
	ball_shader->AddProperty("Specular Color", "uSpecularColor");
	ball_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	ball_shader->Compile();*/
	ball_shader = new SingleLightShader("Shaders/specular.vert", "Shaders/specular.frag");
	ball_shader->AddProperty("Diffuse Color", "uColor");
	ball_shader->AddProperty("Specular Color", "uSpecularColor");
	ball_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	ball_shader->Compile();
	//*********************RED BALL**********************
	red_mat = new Material(ball_shader);
	red_mat->SetPropertyValue("Diffuse Color", Color::Red);
	red_mat->SetPropertyValue("Specular Color", Color::White);

	
	Entity* redBall = LoadPrimitive(Model::SPHERE);
	redBall->GetTransform()->SetPosition(Vector3D(-1.f, 5.f, 0.f));
	ApplyMaterial(redBall, red_mat);
	RigidBody* rigid = new RigidBody();
	rigid->SetVelocity(Vector3D(5.f, 5.f, 0.f));
	redBall->AddComponent(rigid);
	redBall->AddComponent(new Buoyant());
	AddEntity(redBall);
	//*********************GREEN BALL**********************
	green_mat = new Material(ball_shader);
	green_mat->SetPropertyValue("Diffuse Color", Color::Green);
	green_mat->SetPropertyValue("Specular Color", Color::White);

	Entity* greenBall = LoadPrimitive(Model::SPHERE);
	greenBall->GetTransform()->SetPosition(Vector3D(4.f, 4.f, 0.f));
	ApplyMaterial(greenBall, green_mat);
	greenBall->AddComponent(new RigidBody(1.f));
	greenBall->AddComponent(new Buoyant());
	AddEntity(greenBall);

	//*********************CONNECTED BALLS**********************
	Entity* b1 = LoadPrimitive(Model::SPHERE);
	Entity* b2 = LoadPrimitive(Model::SPHERE);
	b1->GetTransform()->SetScale(0.5f);
	b2->GetTransform()->SetScale(0.5f);
	b1->GetTransform()->SetPosition(Vector3D(4.f, 4.f, -2.f));
	b2->GetTransform()->SetPosition(Vector3D(3.f, 2.f, 0.f));
	orange_mat = new Material(ball_shader);
	orange_mat->SetPropertyValue("Diffuse Color", Color(1.f, 0.5f, 0.f));
	orange_mat->SetPropertyValue("Specular Color", Color::White);
	ApplyMaterial(b1, orange_mat);
	ApplyMaterial(b2, orange_mat);
	RigidBody* b1RigidBody = new RigidBody();
	b1RigidBody->SetVelocity(Vector3D(0.f, 3.f, 0.f));
	b1->AddComponent(b1RigidBody);
	b1->AddComponent(new Buoyant());
	RigidBody* b2RigidBody = new RigidBody();
	b2RigidBody->SetVelocity(Vector3D(0.f, 3.f, 3.f));
	b2->AddComponent(b2RigidBody);
	b2->AddComponent(new Buoyant());
	free_spring = new Spring(b1RigidBody, b2RigidBody, 3.f);

	AddEntity(b1);
	AddEntity(b2);
	//*********************HOOKED BALL**********************
	Entity* hookedBall = LoadPrimitive(Model::SPHERE);
	white_mat = new Material(ball_shader);
	white_mat->SetPropertyValue("Diffuse Color", Color::White);
	white_mat->SetPropertyValue("Specular Color", Color::White);
	ApplyMaterial(hookedBall, white_mat);
	hookedBall->GetTransform()->SetPosition(Vector3D(7.f, 5.f, 0.f));
	RigidBody* hookedRB = new RigidBody(2.f);
	hookedBall->AddComponent(hookedRB);
	hookedBall->AddComponent(new Buoyant());
	fixed_spring = new Spring(Vector3D(9.f, 8.f, 0.f), hookedRB, 2.f);
	fixed_spring->coef = 5.f;
	AddEntity(hookedBall);
	
	//*********************WATER**********************
	water_shader = new LightsShader();
	water_shader->AddProperty("Color", "uDiffuseColor");
	water_shader->AddProperty("Transparency", "uTransparency", 1.f);
	water_shader->Compile();
	water_mat = new Material(water_shader);
	water_mat->SetPropertyValue("Transparency", 0.65f);
	water_mat->SetPropertyValue("Color", Color::Blue);
	water_mat->EnableTransparency(true);

	Entity* water = LoadPrimitive(Model::PLANE);
	water->GetTransform()->SetScale(500.f);
	ApplyMaterial(water, water_mat);
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
	DemoScene::Stop();
}

void Ocean::Update(float sec) {
	free_spring->Update();
	fixed_spring->Update();
	DemoScene::Update(sec);
}