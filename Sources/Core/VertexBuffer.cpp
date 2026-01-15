#include "VertexBuffer.h"

using namespace cross;

bool VertexBuffer::HasTextureCoordinates() const {
	return uv_enabled;
}

bool VertexBuffer::HasNormals() const {
	return normals_enabled;
}

bool VertexBuffer::HasTangents() const {
	return tangents_enabled;
}

bool VertexBuffer::HasBitangents() const {
	return bitangents_enabled;
}

U32 VertexBuffer::GetPositionsOffset() const {
	return 0;
}

U32 VertexBuffer::GetTextureCoordinatesOffset() const {
	return GetPositionsOffset() + (uv_enabled ? 3 : 0);
}

U32 VertexBuffer::GetNormalsOffset() const {
	return GetTextureCoordinatesOffset() + (normals_enabled ? 2 : 0);
}

U32 VertexBuffer::GetTangentsOffset() const {
	return GetNormalsOffset() + (tangents_enabled ? 3 : 0);
}

U32 VertexBuffer::GetBitangentsOffset() const {
	return GetTangentsOffset() + (bitangents_enabled ? 3 : 0);
}

void VertexBuffer::UVEnabled(bool enabled) {
	uv_enabled = enabled;
}

void VertexBuffer::NormalsEnabled(bool enabled) {
	normals_enabled = enabled;
}

void VertexBuffer::TangentsEnabled(bool enabled) {
	tangents_enabled = enabled;
}

void VertexBuffer::BitangentsEnabled(bool enabled) {
	bitangents_enabled = enabled;
}

U32 VertexBuffer::VertexSize() const {
	return (GetBitangentsOffset() + 3) * sizeof(float);
}

U32 VertexBuffer::GetDataSize() const {
	return (U32)data.Size();
}

void VertexBuffer::PushData(const Byte* bytes, U32 size) {
	for(U32 i = 0; i < size; i++) {
		data.Add(bytes[i]);
	}
}

Byte* VertexBuffer::GetData() {
	return data.GetData();
}

void VertexBuffer::Free() {
	data.Clear();
}

VertexBuffer* VertexBuffer::Clone() const {
	return CREATE VertexBuffer(*this);
}
