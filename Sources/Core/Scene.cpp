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
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, sys->GetAspectRatio(), 0.1f, config->GetViewDistance());
	camera = new Camera(projection);

	resize_del = sys->WindowResized.Connect(this, &Scene::WindowResizeHandle);
}

void Scene::Update(float sec){
	Screen::Update(sec);
	camera->Update(sec);
	for(Entity* e : objects){
		e->Update(sec);
	}
}

void Scene::Stop(){
	delete camera;
	sys->WindowResized.Disconnect(resize_del);
	for(Entity* e : objects){
		delete e;
	}
	objects.clear();
	Screen::Stop();
}

void Scene::SetCameraViewDistance(float distance){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, sys->GetAspectRatio(), 0.1f, distance);
	camera->SetProjectionMatrix(projection);
}

Entity* Scene::GetEntity(const string& name){
	for(Entity* e : objects){
		if(e->GetName() == name){
			return e;
		}
		Entity* child = e->FindChild(name);
		if(child){
			return child;
		}
	}
	throw CrossException("Can not find entity %s", name.c_str());
}

void Scene::AddEntity(Entity* entity){
	entity->Initialize();
	objects.push_back(entity);
}

Entity* Scene::RemoveEntity(const string& name){
	for(auto it = objects.begin(); it != objects.end(); it++){
		Entity* e = (*it);
		if(e->GetName() == name){
			e->Remove();
			objects.erase(it);
			return e;
		}else{
			return e->RemoveChild(name);
		}
	}
}

List<Light*>& Scene::GetLights(){
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
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, sys->GetAspectRatio(), 0.1f, config->GetViewDistance());
	camera->SetProjectionMatrix(projection);
}