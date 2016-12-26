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

class VertexBuffer{
public:
	
	VertexBuffer();

	bool HasTextureCoordinates();
	bool HasNormals();
	bool HasTangents();
	bool HasBitangents();
	U32 GetPossitionsOffset();
	U32 GetTextureCoordinatesOffset();
	U32 GetNormalsOffset();
	U32 GetTangentsOffset();
	U32 GetBitangentsOffset();
	U32 VertexSize();

	void UVEnabled(bool enabled);
	void NarmalsEnabled(bool enabled);
	void TangentsEnabled(bool enabled);
	void BitangentsEnabled(bool enabled);
	void PushData(const Byte* data, U32 size);
	Byte* GetData();
	U32 GetDataSize();
	void Free();
	VertexBuffer* Clone();

private:
	bool initialized;
	Array<Byte> data;
	bool uv_enabled;
	bool normals_enabled;
	bool tangents_enabled;
	bool bitangents_enabled;

	VertexBuffer(VertexBuffer& obj);
};

}