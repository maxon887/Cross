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
#include "Scenes/Physics/HardConstraints.h"
#include "Utils/PrimitiveDrawer.h"
#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Camera.h"
#include "Shaders/LightsShader.h"
#include "Physics/RigidBody.h"
#include "Physics/Collider.h"
#include "System.h"

Connection::Connection() :
	color(Color::Red)
{ }

void Connection::SetColor(const Color& c){
	color = c;
}

CableConstraint::CableConstraint(float lenght, Vector3D anchor, RigidBody* b) :
	Entity("CableConstraint"),
	length(lenght),
	anchor(anchor),
	body(b)
{ }

void CableConstraint::Update(float sec){
	PrimitiveDrawer::DrawLine(anchor, body->GetPosition(), color);
}

void CableConstraint::Provide(Array<Collision>& collisions, Array<Collider*>& colliders){
	Vector3D BA = anchor - body->GetPosition();
	if(BA.Length() > length){
		Vector3D normal = BA.GetNormalized();
		float depth = BA.Length() - length;
		float restitution = 0.5f;
		Collision collision(body, normal, depth, restitution);
		collisions.Add(collision);
	}
}

Cable::Cable(RigidBody* a, RigidBody* b) :
	Entity("Cable"),
	endA(a),
	endB(b)
{
	Vector3D ab = a->GetPosition() - b->GetPosition();
	length = ab.Length();
}

void Cable::Update(float sec) {
	PrimitiveDrawer::DrawLine(endA->GetPosition(), endB->GetPosition(), color);
}

void Cable::Provide(Array<Collision>& collisions, Array<Collider*>& colliders) {
	Vector3D ab = endB->GetPosition() - endA->GetPosition();
	if(ab.Length() > length) {
		Vector3D normal = ab.GetNormalized();
		float depth = ab.Length() - length;
		float restitution = 0.3f;
		Collision collision(endA, endB, normal, depth, restitution);
		collisions.Add(collision);
	}
}

void Cable::SetLength(float len) {
	length = len;
}

Rod::Rod(RigidBody* a, RigidBody* b) :
	Entity("Rod"),
	endA(a),
	endB(b)
{ 
	Vector3D ab = a->GetPosition() - b->GetPosition();
	length = ab.Length();
}

void Rod::Update(float sec) {
	PrimitiveDrawer::DrawLine(endA->GetPosition(), endB->GetPosition(), color);
}

void Rod::Provide(Array<Collision>& collisions, Array<Collider*>& colliders) {
	Vector3D ab = endB->GetPosition() - endA->GetPosition();
	if(ab.Length() != length){
		Vector3D normal = ab.GetNormalized();
		float depth = ab.Length() - length;
		float restitution = 0.f;
		if(ab.Length() < length){
			normal *= -1.f;
			depth *= -1.f;
		}
		Collision collision(endA, endB, normal, depth, restitution);
		collisions.Add(collision);
	}
}

RigidBody* Rod::GetEndA(){
	return endA;
}

RigidBody* Rod::GetEndB(){
	return endB;
}

void HardConstraints::Start(){
	DemoScene::Start();
	SetBackground(Color(0.3f));
	GetCamera()->SetPosition(Vector3D(-2.f, 2.3f, -5.f));
	GetCamera()->GetEntity()->GetComponent<Transform>()->LookAt(Vector3D::Zero);

	//***************LIGHT*****************
	Entity* light = new Entity("PointLight");
	light->AddComponent(new Transform());
	light->AddComponent(new Light(Light::Type::POINT));
	light->GetComponent<Transform>()->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddEntity(light);
	//***************PARTICLE*****************
	particle_shader = new LightsShader("Shaders/specular.vert", "Shaders/specular.frag");
	particle_shader->AddProperty("Diffuse Color", "uColor");
	particle_shader->AddProperty("Specular Color", "uSpecularColor");
	particle_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	particle_shader->Compile();

	particle_mat = new Material(particle_shader);
	particle_mat->SetPropertyValue("Diffuse Color", Color::Red);
	particle_mat->SetPropertyValue("Specular Color", Color::White);

	//***************PARTICLE*****************
	Entity* particle = LoadPrimitive(Model::Primitive::SPHERE);
	particle->GetComponent<Transform>()->SetScale(0.1f);
	particle->GetComponent<Transform>()->SetPosition(Vector3D(0.f, 3.f, 0.f));
	ApplyMaterial(particle, particle_mat);

	RigidBody* rigid = new RigidBody();
	particle->AddComponent(rigid);
	Collider* collider = new Collider();
	particle->AddComponent(collider);

	AddEntity(particle);
	//***************CABLE*****************
	Entity* connectedObject = LoadPrimitive(Model::Primitive::SPHERE);
	connectedObject->GetComponent<Transform>()->SetScale(0.1f);
	connectedObject->GetComponent<Transform>()->SetPosition(Vector3D(0.f, 2.f, 0.f));
	ApplyMaterial(connectedObject, particle_mat);

	RigidBody* connectedRigid = new RigidBody();
	connectedObject->AddComponent(connectedRigid);
	Collider* connectedCollider = new Collider();
	connectedObject->AddComponent(connectedCollider);
	
	AddEntity(connectedObject);

	CableConstraint* cable = new CableConstraint(2.f, Vector3D(1.f, 3.f, 1.f), connectedRigid);
	AddEntity(cable);
	physics->RegisterCollisionProvider(cable);
	//***************ROD*****************
	rodA = LoadPrimitive(Model::Primitive::SPHERE);
	rodB = LoadPrimitive(Model::Primitive::SPHERE);
	rodA->GetComponent<Transform>()->SetScale(0.1f);
	rodB->GetComponent<Transform>()->SetScale(0.1f);
	rodA->GetComponent<Transform>()->SetPosition(Vector3D(-1.f, 4.f, -1.f));
	rodB->GetComponent<Transform>()->SetPosition(Vector3D(0.f, 5.f, -1.f));
	ApplyMaterial(rodA, particle_mat);
	ApplyMaterial(rodB, particle_mat);

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
	road_shader = new LightsShader();
	road_shader->AddMacro("USE_DIFFUSE_MAP");
	road_shader->AddMacro("USE_TILLING_FACTOR");
	road_shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	road_shader->AddProperty("Transparency", "uTransparency");
	road_shader->AddProperty("Tilling Factor", "uTillingFactor", 1.f);
	road_shader->AddProperty("Specular", "uSpecular", 0.5f);
	road_shader->AddProperty("Shininess", "uShininess", 0.5f * 128.f);
	road_shader->Compile();

	Texture* roadDiffuse = GetTexture("Textures/RoadDiffuse.png");
	roadDiffuse->SetTilingMode(Texture::TilingMode::REPEAT);
	road_mat = new Material(road_shader);
	road_mat->SetPropertyValue("Diffuse Texture", roadDiffuse);
	road_mat->SetPropertyValue("Tilling Factor", 3.f);
	road_mat->SetPropertyValue("Transparency", 0.75f);
	road_mat->EnableTransparency(true);
	Entity* road = LoadPrimitive(Model::Primitive::PLANE);
	road->GetComponent<Transform>()->SetScale(15.f);
	ApplyMaterial(road, road_mat);
	AddEntity(road);

	physics->RegisterCollisionProvider(this);
}

void HardConstraints::Stop(){
	delete particle_shader;
	delete particle_mat;
	delete road_mat;
	delete road_shader;
	DemoScene::Stop();
}

void HardConstraints::Update(float sec){
	DemoScene::Update(sec);
}

void HardConstraints::Provide(Array<Collision>& collisions, Array<Collider*>& colliders){
	for(Collider* collider : colliders){
		if(collider->GetPosition().y < 0 && collider->GetEntity()->HasComponent<RigidBody>()){
			RigidBody* rigid = collider->GetEntity()->GetComponent<RigidBody>();
			Vector3D normal = Vector3D::Up;
			float restitution = 1.f;
			float depth = -collider->GetPosition().y;
			Collision collision(rigid, normal, depth, restitution);
			collisions.Add(collision);
		}
	}
}