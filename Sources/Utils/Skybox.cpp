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
#include "Graphics3D.h"
#include "Shaders/Shader.h"
#include "Model.h"
#include "Material.h"
#include "Config.h"

using namespace cross;

Skybox::Skybox( Cubemap* cubemap ) :
	cubemap(cubemap)
{
	shader = new Shader("Engine/Shaders/skybox.vert", "Engine/Shaders/skybox.frag");
	Shader::Property* prop = new Shader::Property("Cubemap", "cubemap");
	prop->SetValue(cubemap);
	shader->AddProperty(prop);
	shader->Compile();

	material = new Material(shader);

	box = gfx3D->LoadPrimitive(Graphics3D::Primitives::CUBE);
	box->SetMaterial(material);
	box->FaceCulling(false);
	box->SetScale(config->GetViewDistance());
}

Skybox::~Skybox(){
	delete cubemap;
	delete shader;
	delete box;
}

void Skybox::Draw(){
	box->Draw();
}