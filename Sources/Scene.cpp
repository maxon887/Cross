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
#include "Launcher.h"

using namespace cross;

Scene::Scene() :
	Screen(),
	ambient_color(Color(0.1f, 0.1f, 0.1f))
{ }

void Scene::Start(){
	Screen::Start();
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera = new Camera(projection);

	window_resize_handle = MakeDelegate(this, &Scene::WindowResizeHandle);
	game->WindowResized += window_resize_handle;
}

void Scene::Stop(){
	delete camera;
	game->WindowResized -= window_resize_handle;
	Screen::Stop();
}

void Scene::AddDirectionalLight(DirectionalLight* light){
	directional_lights.push_back(light);
}

void Scene::AddPointLight(PointLight* light){
	point_lights.push_back(light);
}

void Scene::AddSpotLight(SpotLight* light){
	spot_lights.push_back(light);
}

CRArray<DirectionalLight*>& Scene::GetDirectionalLights(){
	return directional_lights;
}

CRArray<PointLight*>& Scene::GetPointLights(){
	return point_lights;
}

CRArray<SpotLight*>&Scene::GetSpotLights(){
	return spot_lights;
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

void Scene::WindowResizeHandle(int width, int height){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera->SetProjectionMatrix(projection);
}