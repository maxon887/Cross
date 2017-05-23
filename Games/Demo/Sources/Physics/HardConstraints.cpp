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
#include "Physics/HardConstraints.h"
#include "Graphics2D.h"
#include "Material.h"
#include "Graphics3D.h"
#include "Entity.h"
#include "Light.h"
#include "Camera.h"
#include "Shaders/LightShader.h"
#include "Physics/RigidBody.h"
#include "Physics/Collider.h"
#include "System.h"

CableConstraint::CableConstraint(float lenght, Vector3D anchor, RigidBody* b) :
	length(lenght),
	anchor(anchor),
	body(b)
{ }

void CableConstraint::Update(float sec){
	gfx3D->DrawLine(anchor, body->GetPosition(), Color::Red);
}

void CableConstraint::Provide(Array<Collision>& collisions, Array<Collider*>& colliders){
	Vector3D BA = anchor - body->GetPosition();
	if(BA.Length() > length){
		Collision collision(body);
		collision.contact.normal = BA.GetNormalized();
		collision.contact.depth = BA.Length() - length;
		collision.contact.restitution = 0.5f;
		collisions.push_back(collision);
	}
}

Cable::Cable(RigidBody* a, RigidBody* b) :
	endA(a),
	endB(b)
{
	Vector3D ab = a->GetPosition() - b->GetPosition();
	length = ab.Length();
}

void Cable::Update(float sec) {
	gfx3D->DrawLine(endA->GetPosition(), endB->GetPosition(), Color::Red);
}

void Cable::Provide(Array<Collision>& collisions, Array<Collider*>& colliders) {
	Vector3D ab = endB->GetPosition() - endA->GetPosition();
	if(ab.Length() > length) {
		Collision collision(endA, endB);
		collision.contact.normal = ab.GetNormalized();
		collision.contact.depth = ab.Length() - length;
		collision.contact.restitution = 0.3f;
		collisions.push_back(collision);
	}
}

void Cable::SetLength(float len) {
	length = len;
}

Rod::Rod(RigidBody* a, RigidBody* b) :
	endA(a),
	endB(b)
{ 
	Vector3D ab = a->GetPosition() - b->GetPosition();
	length = ab.Length();
}

void Rod::Update(float sec) {
	gfx3D->DrawLine(endA->GetPosition(), endB->GetPosition(), Color::Red);
}

void Rod::Provide(Array<Collision>& collisions, Array<Collider*>& colliders) {
	Vector3D ab = endB->GetPosition() - endA->GetPosition();
	if(ab.Length() != length){
		Collision collision(endA, endB);
		collision.contact.normal = ab.GetNormalized();
		collision.contact.depth = ab.Length() - length;
		collision.contact.restitution = 0.f;
		if(ab.Length() < length){
			collision.contact.normal *= -1.f;
			collision.contact.depth *= -1.f;
		}
		collisions.push_back(collision);
	}
}

RigidBody* Rod::GetEndA(){
	return endA;
}

RigidBody* Rod::GetEndB(){
	return endB;
}

void HardConstraints::Start(){
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

	//***************PARTICLE*****************
	Entity* particle = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	particle->SetScale(0.1f);
	particle->SetPosition(Vector3D(0.f, 3.f, 0.f));
	gfx3D->AdjustMaterial(particle, particle_mat);

	RigidBody* rigid = new RigidBody();
	particle->AddComponent(rigid);
	Collider* collider = new Collider();
	particle->AddComponent(collider);

	AddEntity(particle);
	//***************CABLE*****************
	Entity* connectedObject = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	connectedObject->SetScale(0.1f);
	connectedObject->SetPosition(Vector3D(0.f, 2.f, 0.f));
	gfx3D->AdjustMaterial(connectedObject, particle_mat);

	RigidBody* connectedRigid = new RigidBody();
	connectedObject->AddComponent(connectedRigid);
	Collider* connectedCollider = new Collider();
	connectedObject->AddComponent(connectedCollider);
	
	AddEntity(connectedObject);

	CableConstraint* cable = new CableConstraint(2.f, Vector3D(1.f, 3.f, 1.f), connectedRigid);
	AddEntity(cable);
	physics->RegisterCollisionProvider(cable);
	//***************ROD*****************
	rodA = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	rodB = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	rodA->SetScale(0.1f);
	rodB->SetScale(0.1f);
	rodA->SetPosition(Vector3D(-1.f, 4.f, -1.f));
	rodB->SetPosition(Vector3D(0.f, 5.f, -1.f));
	gfx3D->AdjustMaterial(rodA, particle_mat);
	gfx3D->AdjustMaterial(rodB, particle_mat);

	RigidBody* rigidA = new RigidBody();
	RigidBody* rigidB = new RigidBody();
	rodA->AddComponent(rigidA);
	rodB->AddComponent(rigidB);
	Collider* colliderA = new Collider();
	Collider* colliderB = new Collider();
	rodA->AddComponent(colliderA);
	rodB->AddComponent(colliderB);
	AddEntity(rodA);
	AddEntity(rodB);

	Rod* rod = new Rod(rigidA, rigidB);
	AddEntity(rod);
	physics->RegisterCollisionProvider(rod);
	//***************ROAD*****************
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
	road_mat->SetPropertyValue("Transparency", 0.75f);
	road_mat->TransparencyEnabled(true);
	Entity* road = gfx3D->LoadPrimitive(Graphics3D::Primitives::PLANE);
	road->SetScale(15.f);
	gfx3D->AdjustMaterial(road, road_mat, false);
	AddEntity(road);

	physics->RegisterCollisionProvider(this);
}

void HardConstraints::Stop(){
	delete road_mat;
	delete road_diffuse;
	delete road_shader;
	CameraControlsScene::Stop();
}

void HardConstraints::Update(float sec){
	CameraControlsScene::Update(sec);
	RigidBody* rigidB = (RigidBody*)rodB->GetComponent(Component::Type::RIGIDBODY);
	//rigidB->ApplyForce(Vector3D(-1.f, 0.f, 0.3f));
}

void HardConstraints::Provide(Array<Collision>& collisions, Array<Collider*>& colliders){
	for(Collider* collider : colliders){
		if(collider->GetPosition().y < 0 && collider->HasComponent(Component::RIGIDBODY)){
			Collision collision((RigidBody*)collider->GetComponent(Component::RIGIDBODY));
			collision.contact.normal = Vector3D::Up;
			collision.contact.depth = -collider->GetPosition().y;
			collisions.push_back(collision);
		}
	}
}