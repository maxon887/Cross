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
#include "VertexBuffer.h"
#include "Scene.h"
#include "Texture.h"
#include "Graphics3D.h"
#include "Entity.h"

using namespace cross;

Mesh::Mesh() :
	Component(Component::Type::MESH),
	vertex_buffer(NULL),
	material(NULL),
	original(true),
	initialized(false),
	face_culling(true)
{ }

Mesh::Mesh(Mesh& obj) :
	Component(Component::Type::MESH),
	index_count(obj.index_count),
	indices(obj.indices),
	material(obj.material), //warning pointer copy!
	VBO(obj.VBO),
	EBO(obj.EBO),
	initialized(obj.initialized),
	face_culling(obj.face_culling),
	original(false)
{
	vertex_buffer = obj.vertex_buffer->Clone();
}

Mesh::~Mesh() {
	delete vertex_buffer;
	if(original && initialized) {
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
}

void Mesh::Update(float sec){
	gfx3D->DrawMesh(this, GetEntity()->GetWorldMatrix());
}

void Mesh::TransferVideoData() {
	index_count = indices.size();

	SAFE(glGenBuffers(1, &VBO));
	SAFE(glGenBuffers(1, &EBO));

	SAFE(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	SAFE(glBufferData(GL_ARRAY_BUFFER, vertex_buffer->GetDataSize(), vertex_buffer->GetData(), GL_STATIC_DRAW));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	SAFE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	vertex_buffer->Free();
	initialized = true;
}

void Mesh::PushData(VertexBuffer* buffer, const Array<GLushort>& inds) {
	if(vertex_buffer == NULL) {
		vertex_buffer = buffer->Clone();
	} else {
		vertex_buffer->PushData(buffer->GetData(), buffer->GetDataSize());
	}

	U32 indsOffset = indices.size();
	for(U32 i = 0; i < inds.size(); ++i) {
		indices.push_back(indsOffset + inds[i]);
	}
}

void Mesh::SetMaterial(Material* mat) {
	this->material = mat;
}

Material* Mesh::GetMaterial() {
	return material;
}

void Mesh::SetFaceCullingEnabled(bool enabled){
	face_culling = enabled;
}

bool Mesh::IsFaceCullingEnabled(){
	return face_culling;
}

void Mesh::SetAlphaBlendingEnabled(bool enabled){
	alpha_blending = enabled;
}

bool Mesh::IsAlphaBlendingEnabled(){
	return alpha_blending;
}

VertexBuffer* Mesh::GetVertexBuffer() {
	return vertex_buffer;
}

Array<GLushort>& Mesh::GetIndices() {
	return indices;
}

Mesh* Mesh::Clone() {
	return new Mesh(*this);
}
