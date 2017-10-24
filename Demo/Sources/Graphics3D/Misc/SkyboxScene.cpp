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
#include "SkyboxScene.h"
#include "Shaders/Shader.h"
#include "Material.h"
#include "Graphics3D.h"
#include "Entity.h"

void SkyboxScene::Start(){
	CameraControlsScene::Start();
	
	Cubemap* cubemap = new Cubemap( "gfx3D/Skybox/right.png",
									"gfx3D/Skybox/left.png",
									"gfx3D/Skybox/up.png",
									"gfx3D/Skybox/down.png",
									"gfx3D/Skybox/back.png",
									"gfx3D/Skybox/front.png" );
	skybox = new Skybox(cubemap);
	shader = GetShader("Engine/Shaders/Texture.sha");
	material = new Material(shader);
	texture = GetTexture("gfx3D/ContainerDiffuse.png");
	material->SetPropertyValue("Texture", texture);
	cube = LoadPrimitive(Graphics3D::Primitives::CUBE);
	gfx3D->AdjustMaterial(cube, material);
	AddEntity(cube);
}

void SkyboxScene::Stop(){

	CameraControlsScene::Stop();
}

void SkyboxScene::Update(float sec){
	CameraControlsScene::Update(sec);
	skybox->Draw();
}