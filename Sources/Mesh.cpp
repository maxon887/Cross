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
#include "Mesh.h"
#include "Game.h"
#include "Scene.h"
#include "Camera.h"
#include "Material.h"
#include "Texture.h"

using namespace cross;

Mesh::Mesh(VertexBuffer* vertexBuffer, CRArray<unsigned int> &indices, unsigned int primitivesCount) :
	vertex_buffer(vertexBuffer),
	primitives_count(primitivesCount),
	material(nullptr)
{
	index_count = indices.size();

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer->GetDataSize(), vertexBuffer->GetData(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vertexBuffer->Free();
}

Mesh::~Mesh(){
}

void Mesh::Draw(){
	if(material == nullptr){
		throw CrossException("Current mesh does not have material");
	}
	Shader* shader = material->GetShader();
	gfxGL->UseShader(shader);

	Camera* cam = game->GetCurrentScene()->GetCamera();
	//binding uniforms
	if(shader->uMVP != -1){
		Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix() * GetModelMatrix();
		mvp = mvp.Transpose();
		SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	}

	if(shader->uColor != -1){
		SAFE(glUniform4fv(shader->uColor, 1, material->GetDiffuseColor().GetData()));
	}

	if(shader->uDiffuseTexture != -1){
		Texture* texture = material->GetDiffuseTexture();
		SAFE(glActiveTexture(GL_TEXTURE0));
		SAFE(glBindTexture(GL_TEXTURE_2D, texture->GetID()));
		SAFE(glUniform1i(shader->uDiffuseTexture, 0));
	}
	//binding attributes
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	unsigned int vertexSize = vertex_buffer->VertexSize();
	if(shader->aPosition != -1){
		SAFE(glEnableVertexAttribArray(shader->aPosition));
		SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)(0 + vertex_buffer->GetPossitionsOffset())));
	}
	if(shader->TextureCoordinatesRequired()){
		if(vertex_buffer->HasTextureCoordinates()){
			SAFE(glEnableVertexAttribArray(shader->aTexCoords));
			SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)(0 + vertex_buffer->GetTextureCoordinatesOffset())));
		}else{
			throw CrossException("Current mesh noes not contain texture coordinates");
		}
	}
	if(shader->NormalsRequired()){
		if(vertex_buffer->HasNormals()){
			SAFE(glEnableVertexAttribArray(shader->aNormal));
			SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)(0 + vertex_buffer->GetNormalsOffset())));
		}else{
			throw CrossException("Current mesh noes not countain normals");
		}
	}
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	//drawing
	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	SAFE(glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Mesh::SetMaterial(Material* mat){
	this->material = mat;
}

unsigned int Mesh::GetPrimitivesCount() const{
	return primitives_count;
}

VertexBuffer* Mesh::GetVertexBuffer(){
	return vertex_buffer;
}