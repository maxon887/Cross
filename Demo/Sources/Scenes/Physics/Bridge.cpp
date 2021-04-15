/*	Copyright � 2015 Lukyanau Maksim

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
#include "Physics/Bridge.h"
#include "Physics/RigidBody.h"
#include "Camera.h"
#include "Light.h"
#include "Shaders/LightsShader.h"
#include "Material.h"
#include "Physics/Collider.h"
#include "System.h"

#include <cmath>

void Bridge::Start(){
	DemoScene::Start();
	SetBackground(Color(0.15f));
	GetCamera()->SetPosition(Vector3D(-4.f, 7.3f, -11.f));
	GetCamera()->GetEntity()->GetComponent<Transform>()->LookAt(Vector3D(0.f, 4.f, 0.f));
	LookAtCamera(Vector3D(0.f, 4.f, 0.f));
	//***************LIGHT*****************
	Entity* light = new Entity("PointLight");
	light->AddComponent(new Transform());
	light->AddComponent(new Light(Light::Type::POINT));
	light->GetComponent<Transform>()->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddEntity(light);

	//***************PARTICLE*****************
	particle_shader = new LightsShader("gfx3D/shaders/specular.vert", "gfx3D/shaders/specular.frag");
	particle_shader->AddProperty("Diffuse Color", "uColor");
	particle_shader->AddProperty("Specular Color", "uSpecularColor");
	particle_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	particle_shader->Compile();

	particle_mat = new Material(particle_shader);
	particle_mat->SetPropertyValue("Diffuse Color", Color::Blue);
	particle_mat->SetPropertyValue("Specular Color", Color::White);

	//Create nodes
	for(U32 i = 0; i < 12; i++){
		Entity* entity = LoadPrimitive(Model::Primitive::SPHERE);
		entity->GetComponent<Transform>()->SetScale(0.2f);
		ApplyMaterial(entity, particle_mat);
		entity->GetComponent<Transform>()->SetPosition(Vector3D((i / 2) * 2.f - 5.f, 4, (i % 2) * 2.f - 1.f));

		RigidBody* rigid = new RigidBody(1.f);
		entity->AddComponent(rigid);

		Collider* collider = new Collider();
		entity->AddComponent(collider);

		AddEntity(entity);
		nodes.push_back(entity);
	}
	//Create cabels
	for(U32 i = 0; i < 10; i++){
		RigidBody* a = (RigidBody*)nodes[i]->GetComponent<RigidBody>();
		RigidBody* b = (RigidBody*)nodes[i + 2]->GetComponent<RigidBody>();
		Cable* cable = Connect(a, b);
		cable->SetColor(Color::Black);
		AddEntity(cable);
	}
	//Supports
	for(U32 i = 0; i < 12; i++){
		Vector3D ancor((i / 2) * 2.2f - 5.5f, 6, (i % 2) * 1.6 - 0.8f);
		RigidBody* obj = (RigidBody*)nodes[i]->GetComponent<RigidBody>();
		CableConstraint* support = NULL;
		if(i < 6){
			support = CreateCable((i / 2) * 0.5f + 3.f, ancor, obj);
		}else{
			support = CreateCable(5.5f - (i / 2) * 0.5, ancor, obj);
		}
		support->SetColor(Color::White);
		AddEntity(support);
	}
	//Rods
	for(U32 i = 0; i < 6; i++){
		RigidBody* a = (RigidBody*)nodes[i * 2]->GetComponent<RigidBody>();
		RigidBody* b = (RigidBody*)nodes[i * 2 + 1]->GetComponent<RigidBody>();
		Rod* rod = CreateRod(a, b);
		rod->SetColor(Color::Red);
		AddEntity(rod);
	}
	//Mass
	mass_mat = new Material(particle_shader);
	mass_mat->SetPropertyValue("Diffuse Color", Color::Green);
	mass_mat->SetPropertyValue("Specular Color", Color::White);

	mass = LoadPrimitive(Model::SPHERE);
	mass->GetComponent<Transform>()->SetScale(0.5f);
	ApplyMaterial(mass, mass_mat);
	AddEntity(mass);
	mass_pos = Vector3D(0.f, 0.f, 0.5f);
}

void Bridge::Stop(){
	DemoScene::Stop();
	delete mass_mat;
	delete particle_mat;
	delete particle_shader;

}

void Bridge::Update(float sec){
	DemoScene::Update(sec);

	const float BaseMass = 1.f;
	const float ExtraMass = 10.f;

	for(U32 i = 0; i < 12; i++){
		RigidBody* rigid = (RigidBody*)nodes[i]->GetComponent<RigidBody>();
		rigid->SetMass(BaseMass);
	}
	
	int x = (int)mass_pos.x;
	float xp = std::fmod(mass_pos.x, 1.f);
	if(x < 0){
		x = 0;
		xp = 0;
	}
	if(x >= 5){
		x = 5;
		xp = 0;
	}

	int z = (int)mass_pos.z;
	float zp = std::fmod(mass_pos.z, 1.f);
	if(z < 0){
		z = 0;
		zp = 0;
	}
	if(z >= 1){
		z = 1;
		zp = 0;
	}

	Vector3D massPoss = nodes[x * 2 + z]->GetComponent<Transform>()->GetPosition() * ((float)(1 - xp)*(1 - zp));
	RigidBody* rigid = (RigidBody*)nodes[x * 2 + z]->GetComponent<RigidBody>();
	rigid->SetMass(BaseMass + ExtraMass * (1 - xp)*(1 - zp));

	if(xp > 0){
		massPoss += nodes[x * 2 + z + 2]->GetComponent<Transform>()->GetPosition() * (float)(xp * (1 - zp));
		rigid = (RigidBody*)nodes[x * 2 + z + 2]->GetComponent<RigidBody>();
		rigid->SetMass(BaseMass + ExtraMass * (xp * (1 - zp)));

		if(zp > 0){
			massPoss += nodes[x * 2 + z + 3]->GetComponent<Transform>()->GetPosition() * (float)(xp * zp);
			rigid = (RigidBody*)nodes[x * 2 + z + 3]->GetComponent<RigidBody>();
			rigid->SetMass(BaseMass + ExtraMass * (xp * zp));
		}
	}
	if(zp > 0){
		massPoss += nodes[x * 2 + z + 1]->GetComponent<Transform>()->GetPosition() * (float)((1 - xp) * zp);
		rigid = (RigidBody*)nodes[x * 2 + z + 1]->GetComponent<RigidBody>();
		rigid->SetMass(BaseMass + ExtraMass * ((1 - xp) * zp));
	}

	mass->GetComponent<Transform>()->SetPosition(massPoss);

	if(input->IsPressed(Key::W)){
		mass_pos += Vector3D(0.f, 0.f, 0.1f);
		if(mass_pos.z > 1.f){
			mass_pos.z = 1.f;
		}
	}
	if(input->IsPressed(Key::S)) {
		mass_pos += Vector3D(0.f, 0.f, -0.1f);
		if(mass_pos.z < 0){
			mass_pos.z = 0;
		}
	}
	if(input->IsPressed(Key::A)) {
		mass_pos += Vector3D(-0.1, 0.f, 0.f);
		if(mass_pos.x < 0){
			mass_pos.x = 0;
		}
	}
	if(input->IsPressed(Key::D)) {
		mass_pos += Vector3D(0.1, 0.f, 0.f);
		if(mass_pos.x > 5){
			mass_pos.x = 5;
		}
	}
}

Rod* Bridge::CreateRod(RigidBody* a, RigidBody* b){
	Rod* rod = new Rod(a, b);
	physics->RegisterCollisionProvider(rod);
	return rod;
}

CableConstraint* Bridge::CreateCable(float len, Vector3D& ancor, RigidBody* obj){
	CableConstraint* cable = new CableConstraint(len, ancor, obj);
	physics->RegisterCollisionProvider(cable);
	return cable;
}

Cable* Bridge::Connect(RigidBody* a, RigidBody* b){
	Cable* cable = new Cable(a, b);
	cable->SetLength(1.9f);
	physics->RegisterCollisionProvider(cable);
	return cable;
}