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

void HardConstraints::Start(){
	CameraControlsScene::Start();
	SetBackground(Color(0.3f));
	GetCamera()->SetPosition(Vector3D(2.f, 2.3f, -5.f));
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

	Entity* particle = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	particle->SetScale(0.1f);
	particle->SetPosition(Vector3D(0.f, 3.f, 0.f));
	gfx3D->AdjustMaterial(particle, particle_mat);

	RigidBody* rigid = new RigidBody();
	particle->AddComponent(rigid);
	Collider* collider = new Collider();
	particle->AddComponent(collider);

	AddEntity(particle);

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
}

void HardConstraints::Provide(Array<Collision>& collisions, Array<Collider*>& colliders){
	for(Collider* collider : colliders){
		if(collider->GetPosition().y < 0){
			Collision collision;
			collision.first = collider;
			Collision::Contact contact;
			contact.normal = Vector3D::Up;
			contact.depth = -collider->GetPosition().y;
			collision.contacts.push_back(contact);
			collisions.push_back(collision);
		}
	}
}