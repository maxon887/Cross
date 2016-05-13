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
#pragma once
#include "Cross.h"
#include "Graphics3D.h"

namespace cross{

class Vertex{
public:
	Vector3D pos;
	Vector2D uv;
	Vector3D normal;
};

class Material{
public:
	static const Material RedPlastic;
	static const Material Bronze;

	Material(Vector3D ambient, Vector3D diffuse, Vector3D specular, float shininess);

	Vector3D ambient;
	Vector3D diffuse;
	Vector3D specular;
	float shininess;
};

class Mesh{
friend Graphics3D;
public:

	Mesh(	CRArray<Vertex> &vertices,
			CRArray<unsigned int> &indices,
			CRArray<Texture*>& diffuseMaps,
			CRArray<Texture*>& specularMaps,
			GLuint polyCount );
	~Mesh();

	GLuint GetPolyCount() const;
	void SetDiffuseTexture(Texture* texture);
	void SetSpecularTexture(Texture* specular);

private:
	GLuint VBO;
	GLuint EBO;
	CRArray<Texture*> diffuse_maps;
	CRArray<Texture*> specular_maps;
	GLuint poly_count;
	GLuint index_count;
};

}