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
#include "Model.h"
#include "Graphics3D.h"
#include "Mesh.h"
#include "Camera.h"
#include "Shaders/SimpleShader.h"
#include "Shaders/TextureShader.h"
#include "Texture.h"

using namespace cross;

Model::Model() :
	diffuse(nullptr),
	shader(nullptr),
	color(Color::White)
{ }

void Model::Draw(){
	if(!shader){
		throw CrossException("Shader not installed");
	}

	SAFE(glEnable(GL_DEPTH_TEST));

	switch(shader->type){
	case Shader::Type::SIMPLE:
		DrawSimple();
		break;
	case Shader::Type::TEXTURE:
		if(!diffuse){
			throw CrossException("There are no texture for model");
		}
		DrawTexture();
		break;
	case Shader::Type::LIGHT:
		DrawLight();
		break;
	default:
		throw CrossException("Wrong shader type");
	}

	SAFE(glDisable(GL_DEPTH_TEST));
}

int Model::GetPolyCount(){
	int polyCount = 0;
	for(Mesh* mesh : meshes){
		polyCount += mesh->poly_count;
	}
	return polyCount;
}


void Model::SetShader(Shader* shader){
	this->shader = shader;
}

void Model::SetMeshes(CRArray<Mesh*>& meshes){
	this->meshes = meshes;
}

void Model::SetDiffuseTexture(Texture* diffuse){
	this->diffuse = diffuse;
}

void Model::SetColor(Color color){
	this->color = color;
}

void Model::DrawSimple(){
	SimpleShader* simpleShader = (SimpleShader*)shader;
	gfxGL->UseShader(simpleShader);

	for(Mesh* mesh : meshes){
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO));

		SAFE(glEnableVertexAttribArray(shader->aPosition));
		SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));

		Camera* camera = gfx3D->GetCamera();
		Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * GetModelMatrix();
		mvp = mvp.Transpose();
		SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
		SAFE(glUniform4fv(simpleShader->uColor, 1, color.GetData()));

		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
		SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}

void Model::DrawTexture(){
	TextureShader* textureShader = (TextureShader*)shader;
	gfxGL->UseShader(textureShader);

	for(Mesh* mesh : meshes){
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO));

		SAFE(glEnableVertexAttribArray(shader->aPosition));
		SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));

		Camera* camera = gfx3D->GetCamera();
		Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * GetModelMatrix();
		mvp = mvp.Transpose();
		SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

		SAFE(glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0));
		SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
		SAFE(glUniform1i(textureShader->uDiffuseTexture, 0));

		SAFE(glEnableVertexAttribArray(textureShader->aDiffuseCoords));
		SAFE(glVertexAttribPointer(textureShader->aDiffuseCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));

		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
		SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}

void Model::DrawLight(){

}