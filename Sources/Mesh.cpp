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

using namespace cross;

Mesh::Mesh(VertexBuffer* vertexBuffer, Array<U32> &indices, U32 primitivesCount) :
	vertex_buffer(vertexBuffer),
	primitives_count(primitivesCount),
	material(NULL),
	original(true),
	name("")
{
	index_count = indices.size();

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer->GetDataSize(), vertexBuffer->GetData(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(U32), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vertexBuffer->Free();
}

Mesh::Mesh(Mesh& obj) : 
	primitives_count(obj.primitives_count),
	index_count(obj.index_count),
	material(obj.material), //warning pointer copy!
	VBO(obj.VBO),
	EBO(obj.EBO),
	original(false)
{
	vertex_buffer = obj.vertex_buffer->Clone();
	name = obj.name + "_copy";
}

Mesh::~Mesh(){
	delete vertex_buffer;
	if(original){
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
}

void Mesh::SetName(const string& name){
	this->name = name;
}

const string& Mesh::GetName(){
	return name;
}

void Mesh::SetMaterial(Material* mat){
	this->material = mat;
}

Material* Mesh::GetMaterial(){
	return material;
}

U32 Mesh::GetPrimitivesCount() const{
	return primitives_count;
}

Mesh* Mesh::Clone(){
	return new Mesh(*this);
}
