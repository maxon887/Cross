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

namespace cross{

/*	Container for vertex data. Used to transfer vertex data between CPU and GPU memory.
	Can be filled dynamically. Supports various vertex properties */
class VertexBuffer {
public:
	/* Returns true if VertexBuffer has UV coordinates in its vertex data */
	bool HasTextureCoordinates() const;
	/* Returns true if VertexBuffer has normals in its vertex data */
	bool HasNormals() const;
	/* Returns true if VertexBuffer has tangents in its vertex data */
	bool HasTangents() const;
	/* Returns true if VertexBuffer has bitangents in its vertex data */
	bool HasBitangents() const;

	/* Gets offset of position data into vertex memory*/
	U32 GetPositionsOffset() const;
	/* Gets offset of position UV coordinates into vertex memory*/
	U32 GetTextureCoordinatesOffset() const;
	/* Gets offset of normals data into vertex memory*/
	U32 GetNormalsOffset() const;
	/* Gets offset of tangents data into vertex memory*/
	U32 GetTangentsOffset() const;
	/* Gets offset of bitangents data into vertex memory*/
	U32 GetBitangentsOffset() const;

	/* Enables or disables texture coordinates in offset calculations */
	void UVEnabled(bool enabled);
	/* Enables or disables normals in offset calculations */
	void NormalsEnabled(bool enabled);
	/* Enables or disables tangents in offset calculations */
	void TangentsEnabled(bool enabled);
	/* Enables or disables bitangents in offset calculations */
	void BitangentsEnabled(bool enabled);

	/* Returns size of one vertex */
	U32 VertexSize() const;
	/* Returns size of a hole vertex buffer */
	U32 GetDataSize() const;
	/* Pushes data into internal memory which will be considered as vertex data */
	void PushData(const Byte* data, U32 size);
	/* Returns flat vertex data. Ready to transfer into GPU*/
	Byte* GetData();
	/* Releases vertex data allocated on CPU */
	void Free();
	/* Creates copy of this class and copy all its vertex data */
	VertexBuffer* Clone() const;

private:
	bool initialized			= false;
	Array<Byte> data			= Array<Byte>();
	bool uv_enabled				= false;
	bool normals_enabled		= false;
	bool tangents_enabled		= false;
	bool bitangents_enabled		= false;
};

}