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

/*	Mesh is smallest 3d entity in game. Which consist of polygons.
	Mesh also contains its materials which need to properly draw it on Scene */
class Mesh : public Component {
public:
	enum StencilBehaviour {
		WRITE,
		READ,
		IGNORED
	};

	Mesh() = default;
	Mesh(Model* model, S32 id);
	~Mesh();

	void Update(float sec) override;
	Mesh* Clone() const override;
	bool Load(tinyxml2::XMLElement* xml, Scene* laodingScene) override;
	bool Save(tinyxml2::XMLElement* xml, tinyxml2::XMLDocument* doc) override;

	void Draw();
	void Draw(Material* material);
	void Draw(Material* material, StencilBehaviour sten);
	void Draw(const Matrix& globalModel, Material* material, StencilBehaviour stencilBehvaiour);

	void TransferVideoData();
	void PushData(VertexBuffer* vertexBuffer, const Array<U16>& indices);

	void SetMaterial(Material* material);
	Material* GetMaterial();

	void SetFaceCullingEnabled(bool enabled);
	bool IsFaceCullingEnabled() const;

	VertexBuffer* GetVertexBuffer();
	Array<U16>& GetIndices();

	S32 GetID() const;
	Model* GetModel();
	U32 GetPolyCount() const;

	bool IsEqual(Mesh* other) const;

protected:
	CROSS_FRIENDLY

	U64 VBO						= 0;
	U64 EBO						= 0;
	VertexBuffer* vertex_buffer = NULL;
	Array<U16> indices			= Array<U16>();

	S32 id						= -1;
	Model* model				= NULL;
	Material* material			= NULL;
	bool original				= true;
	bool initialized			= false;
	bool face_culling			= true;

	void Copy(const Mesh* m);
};

}