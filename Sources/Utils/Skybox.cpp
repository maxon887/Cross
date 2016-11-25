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

Skybox::Skybox( const string& right,
				const string& left,
				const string& top,
				const string& bottom,
				const string& back,
				const string& front	)
{
	SAFE(glGenTextures(1, &textureID));
	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));

	int width, height, channels;
	Byte* image;

	image = gfx2D->LoadRawData(right, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete[] image;
	image = gfx2D->LoadRawData(left, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete image;
	image = gfx2D->LoadRawData(top, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete image;
	image = gfx2D->LoadRawData(bottom, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete image;
	image = gfx2D->LoadRawData(back, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete image;
	image = gfx2D->LoadRawData(front, width, height, channels);
	SAFE(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
	//delete image;
	
	SAFE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	SAFE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	SAFE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    SAFE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	SAFE(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

	shader = new Shader("Engine/Shaders/skybox.vert", "Engine/Shaders/skybox.frag");
	Shader::Property* prop = new Shader::Property("Cubemap", "cubemap");
	prop->SetValueCubemap(textureID);
	shader->AddProperty(prop);
	shader->Compile();

	material = new Material(shader);

	box = gfx3D->LoadPrimitive(Graphics3D::Primitives::CUBE);
	box->SetMaterial(material);
	box->FaceCulling(false);
	box->SetScale(config->GetViewDistance());
}

Skybox::~Skybox(){
	delete shader;
	delete box;
	SAFE(glDeleteTextures(1, &textureID));
}

void Skybox::Draw(){
	box->Draw();
}