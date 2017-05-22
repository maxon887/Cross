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
	GetCamera()->SetPosition(Vector3D(-2.f, 2.3f, -5.f));
	GetCamera()->LookAt(Vector3D::Zero);
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


	Rod* r1 = CreateRod(Vector3D(-1.f, 1.f, -1.f), Vector3D(-1.f, 1.f, 1.f));
	cables.push_back(CreateCable(2.f, Vector3D(-1.f, 2.f, -1.f), r1->GetEndA()));
	cables.push_back(CreateCable(2.f, Vector3D(-1.f, 2.f, 1.f), r1->GetEndB()));

	Rod* r2 = CreateRod(Vector3D(1.f, 1.f, -1.f), Vector3D(1.f, 1.f, 1.f));
	cables.push_back(CreateCable(2.f, Vector3D(1.f, 2.f, -1.f), r2->GetEndA()));
	cables.push_back(CreateCable(2.f, Vector3D(1.f, 2.f, 1.f), r2->GetEndB()));

	Connect(r1->GetEndA(), r2->GetEndA());
	Connect(r1->GetEndB(), r2->GetEndB());

	Rod* r3 = CreateRod(Vector3D(-3.f, 1.f, -1.f), Vector3D(-1.f, 1.f, 1.f));
	cables.push_back(CreateCable(1.7f, Vector3D(-3.f, 2.f, -1.f), r3->GetEndA()));
	cables.push_back(CreateCable(1.7f, Vector3D(-3.f, 2.f, 1.f), r3->GetEndB()));
	Connect(r1->GetEndA(), r3->GetEndA());
	test = Connect(r1->GetEndB(), r3->GetEndB());
}

void Bridge::Stop(){
	CameraControlsScene::Stop();
}

void Bridge::Update(float sec){
	CameraControlsScene::Update(sec);
	Vector3D v = test->GetEndA()->GetPosition() - test->GetEndB()->GetPosition();
	cross::system->LogIt("Len - %f", v.Length());
}

Rod* Bridge::CreateRod(Vector3D& a, Vector3D& b){
	Entity* endA = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	Entity* endB = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	endA->SetScale(0.1f);
	endB->SetScale(0.1f);
	endA->SetPosition(a);
	endB->SetPosition(b);
	gfx3D->AdjustMaterial(endA, particle_mat);
	gfx3D->AdjustMaterial(endB, particle_mat);

	RigidBody* rigidA = new RigidBody();
	RigidBody* rigidB = new RigidBody();
	endA->AddComponent(rigidA);
	endB->AddComponent(rigidB);
	Collider* colliderA = new Collider();
	Collider* colliderB = new Collider();
	endA->AddComponent(colliderA);
	endB->AddComponent(colliderB);
	AddEntity(endA);
	AddEntity(endB);

	Rod* rod = new Rod(colliderA, colliderB);
	AddEntity(rod);
	physics->RegisterCollisionProvider(rod);
	return rod;
}

Cable* Bridge::CreateCable(float len, Vector3D& ancor, Collider* obj){
	Cable* cable = new Cable(len, ancor, obj);
	AddEntity(cable);
	physics->RegisterCollisionProvider(cable);
	return cable;
}

Rod* Bridge::Connect(Collider* a, Collider* b){
	Rod* rod = new Rod(a, b);
	AddEntity(rod);
	physics->RegisterCollisionProvider(rod);
	return rod;
}