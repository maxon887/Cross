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
#include "Component.h"

namespace cross{

class VertexBuffer;

/*	Mesh is smallest 3d entity in game. Which consist of polygons.
	Mesh also contains its materials which need to properly draw it on Scene */
class Mesh : public Component{
public:
	Mesh();
	~Mesh();

	virtual void Update(float sec);

	void TransferVideoData();
	void PushData(VertexBuffer* vertexBuffer, const Array<GLushort>& indices);

	void SetMaterial(Material* material);
	Material* GetMaterial();

	void SetFaceCullingEnabled(bool enabled);
	bool IsFaceCullingEnabled();
	void SetAlphaBlendingEnabled(bool enabled);
	bool IsAlphaBlendingEnabled();

	VertexBuffer* GetVertexBuffer();
	Array<GLushort>& GetIndices();

	Mesh* Clone();

protected:
	CROSS_FRIENDLY

	GLuint VBO;
	GLuint EBO;

	VertexBuffer* vertex_buffer;
	Array<GLushort> indices;
	Material* material;
	U32 index_count;
	bool original;
	bool initialized;
	bool face_culling;
	bool alpha_blending;

	Mesh(Mesh& obj);
};

}