/*	Copyright © 2018 Maksim Lukyanov

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
#include "Utils/Cubemap.h"

using namespace cross;

Skybox::Skybox(Cubemap* cubemap) :
	cubemap(cubemap)
{
	Scene* scene = game->GetCurrentScene();
	box = scene->LoadPrimitive(Model::Primitive::CUBE);
	box->SetName("Skybox");
	box->GetTransform()->SetScale(game->GetCurrentScene()->GetCamera()->GetViewDistance());

	shader = CREATE Shader("Engine/Shaders/Sources/Skybox.vert", "Engine/Shaders/Sources/Skybox.frag");

	shader->AddUniform("Cubemap", "uCubemap", cubemap);
	String MVPPropertyString = "Custom MVP";
	Shader::Uniform customMVPProp(MVPPropertyString, "uCustomMVP");
	shader->AddUniform(customMVPProp);
	shader->Compile();
	mvpID = shader->GetUniform(MVPPropertyString)->GetID();

	material = CREATE Material(shader);
	Mesh* mesh = box->GetComponent<Mesh>();
	mesh->Initialize();
	mesh->SetMaterial(material);
	mesh->EnableFaceCulling(false);
}

Skybox::~Skybox() {
	delete cubemap;
	delete shader;
	delete material;
	delete box;
}

void Skybox::Draw() {
	Camera* cam = game->GetCurrentScene()->GetCamera();
	Matrix view = cam->GetViewMatrix();
	view.m[0][3] = 0.f;
	view.m[1][3] = 0.f;
	view.m[2][3] = 0.f;
	Matrix mvp = cam->GetProjectionMatrix() * view * box->GetTransform()->GetModelMatrix();
	mvp = mvp.GetTransposed();
	material->SetPropertyValue((GLuint)mvpID, mvp);
	Mesh* mesh = box->GetComponent<Mesh>();
	mesh->Draw(mvp, material, Mesh::StencilBehaviour::IGNORED);
}