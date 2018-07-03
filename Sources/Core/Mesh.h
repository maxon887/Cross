/*	Copyright © 2018 Maksim Lukyanov

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
#include "Component.h"

namespace cross{

class VertexBuffer;

/*	A polygon Mesh is a collection of vertices and indices that defines the shape of a polyhedral object.
	To draw a Mesh it must have Material applied. Meshes can be loaded by Scene::GetModel() function */
class Mesh : public Component {
public:
	/*	Determines how Mesh will interact with stencil buffer while drawing */
	enum StencilBehaviour {
		/*	Mesh will write value into stencil buffer on Draw() */
		WRITE,
		/*	Mesh will try to read value from stencil buffer on Draw() */
		READ,
		/*	Mesh don't use stencil buffer. Default behavior */
		IGNORED
	};

	Mesh();
	Mesh(const String& modelfilename, S32 id);
	~Mesh();

	void Initialize(Scene* scene) override;
	/* Will be drawn on update */
	void Update(float sec) override;
	/* Creates new Mesh from this Mesh's data */
	Mesh* Clone() const override;

	/* Draws Mesh on scene */
	void Draw();
	/* Draws Mesh on scene with provided material */
	void Draw(Material* material);
	/* Draws Mesh on scene with special stencil behavior */
	void Draw(Material* material, StencilBehaviour sten);
	/* Draws Mesh on scene with provided MVP matrix and others parameters */
	void Draw(const Matrix& globalModel, Material* material, StencilBehaviour stencilBehaviour);

	/* Transfers Mesh data currently stored in CPU memory into GPU. CPU data will be freed */
	void TransferVideoData();
	/* Add new data to this Mesh or push it on top if have some */
	void PushData(VertexBuffer* vertexBuffer, const Array<U16>& indices);

	/* Sets Material for this mesh. Material must be set in order to draw Mesh */
	void SetMaterial(Material* material);
	/* Gets Material applied for this Mesh */
	Material* GetMaterial();

	/* Returns true if face culling enabled */
	bool IsFaceCullingEnabled() const;
	/* Enables face culling for mesh. If enabled triangles that oriented not in view won't be drawn */
	void EnableFaceCulling(bool yes);

	/* Returns vertex buffer of this Mesh. Can be empty if data already transferred into GPU */
	VertexBuffer* GetVertexBuffer();
	/* Returns array of indices for this Mesh */
	Array<U16>& GetIndices();

	/* Returns unique identifier of this Mesh in Model or -1 if there aren't*/
	S32 GetID() const;
	/* Returns number of triangles in this Mesh */
	U32 GetPolyCount() const;

	/* Returns true if this mesh equal to other */
	bool IsEqual(Mesh* other) const;

private:
	Property<S32> id					= Property<S32>(this, "ID", -1);
	Property<String> model_filename		= Property<String>(this, "Model");
	Property<String> material_filename	= Property<String>(this, "Material");

	U64 VBO								= 0;
	U64 EBO								= 0;
	VertexBuffer* vertex_buffer			= nullptr;
	Array<U16> indices					= Array<U16>();
	Material* material					= nullptr;
	bool original						= true;
	bool initialized					= false;
	bool face_culling					= true;

	void Copy(const Mesh* m);
};

}