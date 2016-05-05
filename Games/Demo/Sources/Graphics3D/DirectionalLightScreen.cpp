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
#include "DirectionalLightScreen.h"
#include "Shaders/Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Graphics2D.h"
#include "Graphics3D.h"

void DirectionalLightScreen::Start(){
	CameraControlScreen::Start();
	orbit_distance = 60.f;

	light = new DirectionalLight(Vector3D(0.f, 0.f, 1.f), Vector3D(0.2f), Vector3D(1.f), Vector3D(0.5f));
	
	Texture* diffuseTexture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse.png", Texture::Filter::TRILINEAR);
	Texture* specularTexture = gfx2D->LoadTexture("gfx3D/ContainerSpecular.png", Texture::Filter::TRILINEAR);
	model = gfx3D->LoadModel("gfx3D/Cube.obj", diffuseTexture, specularTexture);

	for(int i = 0; i < 10; ++i){
		Model* clone = model->Clone();
		clone->SetPosition(Vector3D(Random(-20.f, 20.f), Random(-20.f, 20.f), Random(-20.f, 20.f)));
		clone->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		objects.push_back(clone);
	}
}

void DirectionalLightScreen::Stop(){
	CameraControlScreen::Stop();
	
	delete model;
	delete light;
	
	for(Model* obj : objects){
		delete obj;
	}
}

void DirectionalLightScreen::Update(float sec){
	for(Model* obj : objects){
		gfx3D->DrawModelDirectLight(obj, light);
	}

	CameraControlScreen::Update(sec);
}