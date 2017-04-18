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
#include "Light.h"
#include "Entity.h"

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

	cross::system->WindowResized.Connect(this, &Scene::WindowResizeHandle);
}

void Scene::Update(float sec){
	Screen::Update(sec);
	camera->Update(sec);
	for(Entity* obj : objects){
		obj->Update(sec);
	}
}

void Scene::Stop(){
	delete camera;
	cross::system->WindowResized.Disconnect(this, &Scene::WindowResizeHandle);
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

void Scene::AddEntity(Entity* entity){
	objects.push_back(entity);
	if(entity->GetComponent(Component::COLLIDER)){
		colliders.push_back((Collider*)entity->GetComponent(Component::COLLIDER));
	}
	if(entity->GetComponent(Component::LIGHT)){
		lights.push_back((Light*)entity->GetComponent(Component::LIGHT));
	}
	for(Entity* child : entity->GetChildren()){
		AddEntity(child);
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