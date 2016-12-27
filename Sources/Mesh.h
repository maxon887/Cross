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
#include "Transformable.h"
#include "GraphicsGL.h"

namespace cross{

class VertexBuffer;

class Mesh : public Transformable{
public:
	Mesh(VertexBuffer* vertexBuffer, Array<U32> &indices, U32 primitivesCount);
	~Mesh();

	void SetName(const string& name);
	const string& GetName();

	void SetMaterial(Material* material);
	Material* GetMaterial();

	U32 GetPrimitivesCount() const;

	Mesh* Clone();

protected:
	CROSS_FRIENDLY

	GLuint VBO;
	GLuint EBO;

	string name;
	VertexBuffer* vertex_buffer;
	Material* material;
	U32 primitives_count;
	U32 index_count;
	bool original;

	Mesh(Mesh& obj);
};

}