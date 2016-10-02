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

VertexBuffer::VertexBuffer():
	initialized(false),
	uv_enabled(false),
	normals_enabled(false)
{ }

VertexBuffer::VertexBuffer(VertexBuffer& obj) :
	initialized(obj.initialized),
	uv_enabled(obj.uv_enabled),
	normals_enabled(obj.normals_enabled),
	data(obj.data)
{ }

bool VertexBuffer::HasTextureCoordinates(){
	return uv_enabled;
}

bool VertexBuffer::HasNormals(){
	return normals_enabled;
}

U32 VertexBuffer::VertexSize(){
	U32 vertSize = 3;
	if(uv_enabled){
		vertSize += 2;
	}
	if(normals_enabled){
		vertSize += 3;
	}
	return vertSize * sizeof(float);
}

U32 VertexBuffer::GetPossitionsOffset(){
	return 0;
}

U32 VertexBuffer::GetTextureCoordinatesOffset(){
	if(uv_enabled){
		return 3;
	}else{
		throw CrossException("VertexBuffer noes not contains texture coordinates");
	}
}

U32 VertexBuffer::GetNormalsOffset(){
	if(normals_enabled){
		if(uv_enabled){
			return 5;
		}else{
			return 3;
		}
	}else{
		throw CrossException("VertexBuffer noes not contains normals");
	}
}

void VertexBuffer::UVEnabled(bool enabled){
	uv_enabled = enabled;
}

void VertexBuffer::NarmalsEnabled(bool enabled){
	normals_enabled = enabled;
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