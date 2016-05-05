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
#include "MultiLightScreen.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Model.h"

void MultiLightScreen::Start(){
	CameraControlScreen::Start();
	orbit_distance = 60.f;

	for(int i = 0; i < 10; ++i){
		PointLight* light = new PointLight(Vector3D(0.2f), Vector3D(1.f), Vector3D(0.85f), 1.f, 0.014f, 0.0007f);
		light->SetPosition(Vector3D(Random(-30.f, 30.f), Random(-30.f, 30.f), Random(-30.f, 30.f)));
		point_lights.push_back(light);
	}

	Texture* diffuseTexture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse.png", Texture::Filter::TRILINEAR);
	Texture* specularTexture = gfx2D->LoadTexture("gfx3D/ContainerSpecular.png", Texture::Filter::TRILINEAR);
	model = gfx3D->LoadModel("gfx3D/Cube.obj", diffuseTexture, specularTexture);

	for(int i = 0; i < 20; ++i){
		Model* clone = model->Clone();
		clone->SetPosition(Vector3D(Random(-20.f, 20.f), Random(-20.f, 20.f), Random(-20.f, 20.f)));
		clone->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		objects.push_back(clone);
	}

}

void MultiLightScreen::Stop(){
	CameraControlScreen::Stop();

}

void MultiLightScreen::Update(float sec){
	for(PointLight* light : point_lights){
		light->Draw();
	}

	for(Model* model : objects){
		gfx3D->DrawModelMultiLight(model, point_lights);
	}

	CameraControlScreen::Update(sec);
}