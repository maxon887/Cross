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

namespace cross{

class VertexBuffer {
public:
	bool HasTextureCoordinates() const;
	bool HasNormals() const;
	bool HasTangents() const;
	bool HasBitangents() const;
	U32 GetPossitionsOffset() const;
	U32 GetTextureCoordinatesOffset() const;
	U32 GetNormalsOffset() const;
	U32 GetTangentsOffset() const;
	U32 GetBitangentsOffset() const;
	U32 VertexSize() const;

	void UVEnabled(bool enabled);
	void NarmalsEnabled(bool enabled);
	void TangentsEnabled(bool enabled);
	void BitangentsEnabled(bool enabled);
	void PushData(const Byte* data, U32 size);
	Byte* GetData();
	U32 GetDataSize() const;
	void Free();
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