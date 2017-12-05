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
#include "Skybox.h"
#include "Shaders/Shader.h"
#include "Entity.h"
#include "Material.h"
#include "Game.h"
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "Transform.h"

using namespace cross;

Skybox::Skybox(Cubemap* cubemap) :
	cubemap(cubemap)
{
	box = game->GetCurrentScene()->LoadPrimitive(Model::Primitive::CUBE);
	box->GetTransform()->SetScale(game->GetCurrentScene()->GetCamera()->GetViewDistance());

	shader = new Shader("Engine/Shaders/Light.vtx", "Engine/Shaders/Light.fgm");
	Shader::Property* cubemapProp = new Shader::Property("Cubemap", "cubemap");
	cubemapProp->SetValue(cubemap);
	shader->AddProperty(cubemapProp);
	Shader::Property* customMVPProp = new Shader::Property("Custom MVP", "uCustomMVP");
	shader->AddProperty(customMVPProp);
	shader->Compile();
	mvpID = customMVPProp->GetID();

	material = new Material(shader);
	box->GetComponent<Mesh>()->SetMaterial(material);
}

Skybox::~Skybox(){
	delete cubemap;
	delete shader;
	delete material;
	delete box;
}

void Skybox::Draw(){
	Camera* cam = game->GetCurrentScene()->GetCamera();
	Matrix view = cam->GetViewMatrix();
	view.m[0][3] = 0.f;
	view.m[1][3] = 0.f;
	view.m[2][3] = 0.f;
	Matrix mvp = cam->GetProjectionMatrix() * view * box->GetTransform()->GetModelMatrix();
	mvp = mvp.GetTransposed();
	material->SetPropertyValue(mvpID, mvp);
	Mesh* mesh = box->GetChildren().front()->GetComponent<Mesh>();
	mesh->Draw(mvp, material, Mesh::StencilBehaviour::IGNORED);
}