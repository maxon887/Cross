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
#include "LightCastersScreen.h"
#include "Shaders/Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Graphics2D.h"
#include "Graphics3D.h"

void LightCastersScreen::Start(){
	CameraControlScreen::Start();
	model = gfx3D->LoadModel(Shader::Type::LIGHT_CASTERS, "gfx3D/Cube.obj");
	Texture* diffuseTexture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse.png", Texture::Filter::TRILINEAR);
	Texture* specularTexture = gfx2D->LoadTexture("gfx3D/ContainerSpecular.png", Texture::Filter::TRILINEAR);
	model->SetDiffuseTexture(diffuseTexture);
	model->SetSpecularTexture(specularTexture);
}

void LightCastersScreen::Stop(){
	CameraControlScreen::Stop();
}

void LightCastersScreen::Update(float sec){
	model->Draw();

	CameraControlScreen::Update(sec);
}