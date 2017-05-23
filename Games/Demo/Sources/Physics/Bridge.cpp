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
#include "Physics/Bridge.h"
#include "Physics/RigidBody.h"
#include "Camera.h"
#include "Light.h"
#include "Shaders/LightShader.h"
#include "Material.h"
#include "Graphics3D.h"
#include "Physics/Collider.h"
#include "System.h"

void Bridge::Start(){
	CameraControlsScene::Start();
	SetBackground(Color(0.3f));
	GetCamera()->SetPosition(Vector3D(-4.f, 7.3f, -11.f));
	GetCamera()->LookAt(Vector3D(0.f, 4.f, 0.f));
	LookAtCamera(Vector3D(0.f, 4.f, 0.f));
	//***************LIGHT*****************
	Entity* light = new Entity();
	light->AddComponent(new Light(Light::Type::POINT));
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddEntity(light);
	//***************PARTICLE*****************
	
	particle_shader = new LightShader("gfx3D/shaders/specular.vert", "gfx3D/shaders/specular.frag");
	particle_shader->AddProperty("Diffuse Color", "uColor");
	particle_shader->AddProperty("Specular Color", "uSpecularColor");
	particle_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	particle_shader->Compile();

	particle_mat = new Material(particle_shader);
	particle_mat->SetPropertyValue("Diffuse Color", Color::Red);
	particle_mat->SetPropertyValue("Specular Color", Color::White);

	//Create nodes
	for(U32 i = 0; i < 12; i++){
		Entity* entity = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
		entity->SetScale(0.1f);
		gfx3D->AdjustMaterial(entity, particle_mat);
		entity->SetPosition(Vector3D((i / 2) * 2.f - 5.f, 4, (i % 2) * 2.f - 1.f));

		RigidBody* rigid = new RigidBody(1.f);
		entity->AddComponent(rigid);

		Collider* collider = new Collider();
		entity->AddComponent(collider);

		AddEntity(entity);
		nodes.push_back(entity);
	}
	//Create cabels
	for(U32 i = 0; i < 10; i++){
		Collider* a = (Collider*)nodes[i]->GetComponent(Component::COLLIDER);
		Collider* b = (Collider*)nodes[i + 2]->GetComponent(Component::COLLIDER);
		Connect(a, b);
	}
	//Supports
	for(U32 i = 0; i < 12; i++){
		Vector3D ancor((i / 2) * 2.2f - 5.5f, 6, (i % 2) * 1.6 - 0.8f);
		Collider* obj = (Collider*)nodes[i]->GetComponent(Component::COLLIDER);
		if(i < 6){
			CreateCable((i / 2) * 0.5f + 3.f, ancor, obj);
		}else{
			CreateCable(5.5f - (i / 2) * 0.5, ancor, obj);
		}
	}
	//Rods
	for(U32 i = 0; i < 6; i++){
		Collider* a = (Collider*)nodes[i * 2]->GetComponent(Component::COLLIDER);
		Collider* b = (Collider*)nodes[i * 2 + 1]->GetComponent(Component::COLLIDER);
		CreateRod(a, b);
	}

	mass = gfx3D->LoadPrimitive(Graphics3D::SPHERE);
	mass->SetScale(0.5f);
	gfx3D->AdjustMaterial(mass, particle_mat);
	AddEntity(mass);

	/*
	Rod* r1 = CreateRod(Vector3D(-1.f, 1.f, -1.f), Vector3D(-1.f, 1.f, 1.f));
	cables.push_back(CreateCable(2.f, Vector3D(-1.f, 2.f, -1.f), r1->GetEndA()));
	cables.push_back(CreateCable(2.f, Vector3D(-1.f, 2.f, 1.f), r1->GetEndB()));
	
	Rod* r2 = CreateRod(Vector3D(1.f, 1.f, -1.f), Vector3D(1.f, 1.f, 1.f));
	cables.push_back(CreateCable(2.f, Vector3D(1.f, 2.f, -1.f), r2->GetEndA()));
	cables.push_back(CreateCable(2.f, Vector3D(1.f, 2.f, 1.f), r2->GetEndB()));
	
	Connect(r1->GetEndA(), r2->GetEndA());
	Connect(r1->GetEndB(), r2->GetEndB());

	Rod* r3 = CreateRod(Vector3D(-3.f, 1.f, -1.f), Vector3D(-3.f, 1.f, 1.f));
	cables.push_back(CreateCable(1.7f, Vector3D(-3.f, 2.f, -1.f), r3->GetEndA()));
	cables.push_back(CreateCable(1.7f, Vector3D(-3.f, 2.f, 1.f), r3->GetEndB()));
	Connect(r1->GetEndA(), r3->GetEndA());
	Connect(r1->GetEndB(), r3->GetEndB());

	Rod* r4 = CreateRod(Vector3D(3.f, 1.f, -1.f), Vector3D(3.f, 1.f, 1.f));
	cables.push_back(CreateCable(1.7f, Vector3D(3.f, 2.f, -1.f), r4->GetEndA()));
	cables.push_back(CreateCable(1.7f, Vector3D(3.f, 2.f, 1.f), r4->GetEndB()));
	Connect(r2->GetEndA(), r4->GetEndA());
	Connect(r2->GetEndB(), r4->GetEndB());

	Rod* r5 = CreateRod(Vector3D(-4.5f, 1.f, -1.f), Vector3D(-4.5f, 1.f, 1.f));
	cables.push_back(CreateCable(1.5f, Vector3D(-4.5f, 2.f, -1.f), r5->GetEndA()));
	cables.push_back(CreateCable(1.5f, Vector3D(-4.5f, 2.f, 1.f), r5->GetEndB()));
	Connect(r3->GetEndA(), r5->GetEndA());
	Connect(r3->GetEndB(), r5->GetEndB());

	Rod* r6 = CreateRod(Vector3D(4.5f, 1.f, -1.f), Vector3D(4.5f, 1.f, 1.f));
	cables.push_back(CreateCable(1.5f, Vector3D(4.5f, 2.f, -1.f), r6->GetEndA()));
	cables.push_back(CreateCable(1.5f, Vector3D(4.5f, 2.f, 1.f), r6->GetEndB()));
	Connect(r4->GetEndA(), r6->GetEndA());
	Connect(r4->GetEndB(), r6->GetEndB());*/
}

void Bridge::Stop(){
	CameraControlsScene::Stop();
}

void Bridge::Update(float sec){
	CameraControlsScene::Update(sec);

	for(U32 i = 0; i < 12; i++){
		RigidBody* rigid = (RigidBody*)nodes[i]->GetComponent(Component::RIGIDBODY);
		rigid->SetMass(1.f);
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

	//float scale = (1 - xp)(1 - zp);
	//Vector3D massPos = nodes[x * 2 + z]->GetPosition() * (float)((1 - xp)(1 - zp));
	Vector3D massPoss = nodes[x * 2 + z]->GetPosition() * ((float)(1 - xp)*(1 - zp));
	//mass->SetPosition(nodes[x * 2 + z]->GetPosition() * ((float)(1 - xp)*(1 - zp)));

	if(xp > 0){
		massPoss += nodes[x * 2 + z + 2]->GetPosition() * (float)(xp * (1 - zp));
		//mass->SetPosition(nodes[x * 2 + z + 2]->GetPosition() * (float)(xp * (1 - zp)));
		if(zp > 0){
			//mass->SetPosition(nodes[x * 2 + z + 3]->GetPosition() * (float)(xp * zp));
			massPoss += nodes[x * 2 + z + 3]->GetPosition() * (float)(xp * zp);
		}
	}
	if(zp > 0){
		//mass->SetPosition(nodes[x * 2 + z + 1]->GetPosition() * (float)((1 - xp) * zp));
		massPoss += nodes[x * 2 + z + 1]->GetPosition() * (float)((1 - xp) * zp);
	}

	mass->SetPosition(massPoss);

	if(input->IsPressed(Key::W)){
		mass_pos += Vector3D(0.f, 0.f, 0.1f);
		//mass->SetPosition(mass->GetPosition() + Vector3D(0.0f, 0.f, 0.1f));
	}
	if(input->IsPressed(Key::S)) {
		//mass->SetPosition(mass->GetPosition() + Vector3D(0.0f, 0.f, -0.1f));
		mass_pos += Vector3D(0.f, 0.f, -0.1f);
	}
	if(input->IsPressed(Key::A)) {
		//mass->SetPosition(mass->GetPosition() + Vector3D(-0.1f, 0.f, 0.0f));
		mass_pos += Vector3D(-0.1, 0.f, 0.f);
	}
	if(input->IsPressed(Key::D)) {
		mass_pos += Vector3D(0.1, 0.f, 0.f);
		//mass->SetPosition(mass->GetPosition() + Vector3D(0.1f, 0.f, 0.0f));
	}
}

Rod* Bridge::CreateRod(Collider* a, Collider* b){
	Rod* rod = new Rod(a, b);
	AddEntity(rod);
	physics->RegisterCollisionProvider(rod);
	return rod;
}

CableConstraint* Bridge::CreateCable(float len, Vector3D& ancor, Collider* obj){
	CableConstraint* cable = new CableConstraint(len, ancor, obj);
	AddEntity(cable);
	physics->RegisterCollisionProvider(cable);
	return cable;
}

Cable* Bridge::Connect(Collider* a, Collider* b){
	Cable* cable = new Cable(a, b);
	cable->SetLength(1.9f);
	AddEntity(cable);
	physics->RegisterCollisionProvider(cable);
	return cable;
}