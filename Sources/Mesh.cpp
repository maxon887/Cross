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

using namespace cross;

const Material Material::RedPlastic(Vector3D(0.3, 0.0, 0.0), Vector3D(0.7, 0.0, 0.0), Vector3D(0.7, 0.6, 0.6), 0.25f);
const Material Material::Bronze(Vector3D(0.2125, 0.1275, 0.054), Vector3D(0.714, 0.4284, 0.3935), Vector3D(0.3935, 0.2719, 0.1666), 0.2f);

Material::Material(Vector3D ambient, Vector3D diffuse, Vector3D specular, float shininess){
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

Mesh::Mesh(CRArray<Vertex> &vertices, CRArray<unsigned int> &indices, GLuint polyCount) : 
	material(Material::RedPlastic)
{
	poly_count = polyCount;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	index_count = indices.size();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint Mesh::GetPolyCount() const{
	return poly_count;
}

GLuint Mesh::GetIndexCount() const{
	return index_count;
}

GLuint Mesh::GetVertexBufferObject() const{
	return VBO;
}

GLuint Mesh::GetElementBufferObjet() const{
	return EBO;
}

Material Mesh::GetMaterial(){
	return material;
}

void Mesh::SetMaterial(Material mat){
	material = mat;
}