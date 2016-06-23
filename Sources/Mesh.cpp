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
#include "Texture.h"

using namespace cross;

const Material Material::RedPlastic(Vector3D(0.3f, 0.0f, 0.0f), Vector3D(0.7f, 0.0f, 0.0f), Vector3D(0.7f, 0.6f, 0.6f), 0.25f);
const Material Material::Bronze(Vector3D(0.2125f, 0.1275f, 0.054f), Vector3D(0.714f, 0.4284f, 0.3935f), Vector3D(0.3935f, 0.2719f, 0.1666f), 0.2f);

Material::Material(Vector3D ambient, Vector3D diffuse, Vector3D specular, float shininess){
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

Mesh::Mesh(VertexBuffer* vertexBuffer, CRArray<unsigned int> &indices, CRArray<Texture*>& diffuseMaps, CRArray<Texture*>& specularMaps, GLuint polyCount) :
	diffuse_maps(diffuseMaps),
	specular_maps(specularMaps)
{
	poly_count = polyCount;
	vertex_buffer = vertexBuffer;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer->GetDataSize(), vertexBuffer->GetData(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	index_count = indices.size();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	vertexBuffer->Free();
}

Mesh::~Mesh(){
	for(Texture* texture : diffuse_maps){
		delete texture;
	}
	for(Texture* texture : specular_maps){
		delete texture;
	}
}

GLuint Mesh::GetPolyCount() const{
	return poly_count;
}

VertexBuffer* Mesh::GetVertexBuffer(){
	return vertex_buffer;
}

void Mesh::SetDiffuseTexture(Texture* texture){
	diffuse_maps.clear();
	diffuse_maps.push_back(texture);
}

void Mesh::SetSpecularTexture(Texture* texture){
	specular_maps.clear();
	specular_maps.push_back(texture);
}