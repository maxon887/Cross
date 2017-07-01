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
#include "VertexBuffer.h"

using namespace cross;

bool VertexBuffer::HasTextureCoordinates(){
	return uv_enabled;
}

bool VertexBuffer::HasNormals(){
	return normals_enabled;
}

bool VertexBuffer::HasTangents(){
	return tangents_enabled;
}

bool VertexBuffer::HasBitangents(){
	return bitangents_enabled;
}

U32 VertexBuffer::GetPossitionsOffset(){
	return 0;
}

U32 VertexBuffer::GetTextureCoordinatesOffset(){
	return GetPossitionsOffset() + (uv_enabled ? 3 : 0);
}

U32 VertexBuffer::GetNormalsOffset(){
	return GetTextureCoordinatesOffset() + (normals_enabled ? 2 : 0);
}

U32 VertexBuffer::GetTangentsOffset(){
	return GetNormalsOffset() + (tangents_enabled ? 3 : 0);
}

U32 VertexBuffer::GetBitangentsOffset(){
	return GetTangentsOffset() + (bitangents_enabled ? 3 : 0);
}

U32 VertexBuffer::VertexSize() {
	return (GetBitangentsOffset() + 3) * sizeof(float);
}

void VertexBuffer::UVEnabled(bool enabled){
	uv_enabled = enabled;
}

void VertexBuffer::NarmalsEnabled(bool enabled){
	normals_enabled = enabled;
}

void VertexBuffer::TangentsEnabled(bool enabled){
	tangents_enabled = enabled;
}

void VertexBuffer::BitangentsEnabled(bool enabled){
	bitangents_enabled = enabled;
}

void VertexBuffer::PushData(const Byte* bytes, U32 size){
	data.insert(data.end(), bytes, bytes+size);
}

Byte* VertexBuffer::GetData(){
	return data.data();
}

U32 VertexBuffer::GetDataSize(){
	return data.size();
}

void VertexBuffer::Free(){
	data.clear();
}

VertexBuffer* VertexBuffer::Clone(){
	return new VertexBuffer(*this);
}