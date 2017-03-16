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
#include "Scene.h"
#include "Camera.h"
#include "Game.h"
#include "System.h"
#include "Config.h"
#include "Model.h"
#include "Light.h"
#include "Entity.h"
#include "Physics/Collider.h"
#include "Physics/RigidBody.h"

using namespace cross;

Scene::Scene() :
	Screen(),
	ambient_color(Color(0.1f, 0.1f, 0.1f))
{ }

void Scene::Start(){
	Screen::Start();
	is_scene = true;
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, system->GetAspectRatio(), 0.1f, config->GetViewDistance());
	camera = new Camera(projection);

	window_resize_handle = MakeDelegate(this, &Scene::WindowResizeHandle);
	cross::system->WindowResized += window_resize_handle;
}

void Scene::Update(float sec){
	Screen::Update(sec);
	camera->Update(sec);
	for(Entity* obj : objects){
		obj->Update(sec);
	}
	ProcessCollisions();
}

void Scene::Stop(){
	delete camera;
	cross::system->WindowResized -= window_resize_handle;
	for(Entity* obj : objects){
		delete obj;
	}
	for(Light* light : lights){
		delete light;
	}
	Screen::Stop();
}

void Scene::SetCameraViewDistance(float distance){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, system->GetAspectRatio(), 0.1f, distance);
	camera->SetProjectionMatrix(projection);
}

void Scene::AddLight(Light* light){
	lights.push_back(light);
}

void Scene::AddEntity(Entity* entity){
	objects.push_back(entity);
	if(entity->GetComponent(Component::Type::COLLIDER)){
		colliders.push_back((Collider*)entity->GetComponent(Component::Type::COLLIDER));
	}
}

Array<Light*>& Scene::GetLights(){
	return lights;
}

Camera* Scene::GetCamera(){
	return camera;
}

Color Scene::GetAmbientColor() const{
	return ambient_color;
}

void Scene::SetAmbientColor(const Color& color){
	this->ambient_color = color;
}

void Scene::WindowResizeHandle(S32 width, S32 height){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, system->GetAspectRatio(), 0.1f, config->GetViewDistance());
	camera->SetProjectionMatrix(projection);
}

void Scene::ProcessCollisions(){
	for(U32 i = 0; i < colliders.size(); ++i){
		for(U32 j = i + 1; j < colliders.size(); ++j){
			Collision collision = colliders[i]->CollisionCheck(colliders[j]);
			if(collision.yes){
				if(colliders[i]->GetType() == Collider::Type::SPHERE &&	colliders[j]->GetType() == Collider::Type::SPHERE){
					RigidBody* objA = (RigidBody*)colliders[i]->GetComponent(Component::Type::RIGIDBODY);
					RigidBody* objB = (RigidBody*)colliders[j]->GetComponent(Component::Type::RIGIDBODY);
					if(objA && objB){
						Vector3D centerV = Vector3D(objA->GetPosition() - objB->GetPosition()).GetNormalized();
						float Vs = Vector3D::Dot((objA->GetVelocity() - objB->GetVelocity()), centerV);
						if(Vs >= 0){
							return;
						}
						float dV = Vs * 2.f;
						float totalIverseMass = objA->GetInverseMass() + objB->GetInverseMass();
						float impulse = dV / totalIverseMass;
						Vector3D impulseV = centerV * impulse * (-1.f);

						objA->AddVelocity(impulseV * objA->GetInverseMass());
						objB->AddVelocity(impulseV * objB->GetInverseMass() * (-1.f));
					}
				}
			}
		}
	}
}