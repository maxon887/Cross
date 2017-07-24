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
#include "Mesh.h"
#include "VertexBuffer.h"
#include "Scene.h"
#include "Texture.h"
#include "Entity.h"
#include "Material.h"
#include "Game.h"
#include "Camera.h"

using namespace cross;

Mesh::Mesh() 
{ }

Mesh::Mesh(const Mesh& obj) :
	Component(obj),
	VBO(obj.VBO),
	EBO(obj.EBO),
	vertex_buffer(obj.vertex_buffer),
	index_count(obj.index_count),
	material(obj.material),
	indices(obj.indices),
	initialized(obj.initialized),
	face_culling(obj.face_culling),
	original(false)
{
	vertex_buffer = obj.vertex_buffer->Clone();
	
}

Mesh::~Mesh() {
	delete vertex_buffer;
	if(original && initialized) {
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
}

void Mesh::Update(float sec){
	Draw();
}

void Mesh::Draw() {
	Draw(material);
}

void Mesh::Draw(Material* mat){
	if(GetEntity()){
		Draw(GetEntity()->GetWorldMatrix(), mat, Graphics3D::StencilBehaviour::IGNORED);
	}else{
		Draw(Matrix::Identity, mat, Graphics3D::StencilBehaviour::IGNORED);
	}
}

void Mesh::Draw(const Matrix& globalModel, Material* material,
				Graphics3D::StencilBehaviour stencilBehvaiour) {
	if(!initialized) {
		throw CrossException("Before draw mesh needs to be initialized");
	}
	if(material == nullptr) {
		throw CrossException("Current mesh does not have material");
	}
	Shader* shader = material->GetShader();
	gfxGL->UseShader(shader);

	Scene* scene = game->GetCurrentScene();
	//binding uniforms
	if(shader->uMVP != -1) {
		Camera* cam = scene->GetCamera();
		Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix() * globalModel;
		mvp = mvp.GetTransposed();
		SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	}

	if(shader->uModelMatrix != -1) {
		Matrix model = globalModel;
		model = model.GetTransposed();
		SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.GetData()));
	}

	if(shader->uNormalMatrix != -1) {
		Matrix normal = (globalModel).GetInversed();
		SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normal.GetData()));
	}

	if(shader->uCameraPosition != -1) {
		SAFE(glUniform3fv(shader->uCameraPosition, 1, scene->GetCamera()->GetPosition().GetData()));
	}

	if(shader->uColor != -1) {
		SAFE(glUniform4fv(shader->uColor, 1, Color::White.GetData()));
	}

	if(shader->uAmbientLight != -1) {
		SAFE(glUniform4fv(shader->uAmbientLight, 1, scene->GetAmbientColor().GetData()));
	}

	for(Shader::Property* prop : material->properties) {
		if(prop->glId == -1) {
			throw CrossException("Broken shader property");
		}
		if(prop->value == NULL) {
			throw CrossException("Property '%s' value not assigned", prop->name.c_str());
		}

		switch(prop->type) {
		case Shader::Property::SAMPLER:
			SAFE(glActiveTexture(GL_TEXTURE0 + material->active_texture_slot));
			SAFE(glBindTexture(GL_TEXTURE_2D, *(GLuint*)prop->value));
			SAFE(glUniform1i(prop->glId, material->active_texture_slot));
			material->active_texture_slot++;
			break;
		case Shader::Property::MAT4:
			SAFE(glUniformMatrix4fv(prop->glId, 1, GL_FALSE, (GLfloat*)prop->value));
			break;
		case Shader::Property::VEC4:
			SAFE(glUniform4fv(prop->glId, 1, (GLfloat*)prop->value));
			break;
		case Shader::Property::VEC3:
			SAFE(glUniform3fv(prop->glId, 1, (GLfloat*)prop->value));
			break;
		case Shader::Property::VEC2:
			SAFE(glUniform2fv(prop->glId, 1, (GLfloat*)prop->value));
			break;
		case Shader::Property::FLOAT:
			SAFE(glUniform1f(prop->glId, *(GLfloat*)(prop->value)));
			break;
		case Shader::Property::INT:
			SAFE(glUniform1f(prop->glId, *(GLfloat*)(prop->value)));
			break;
		case Shader::Property::CUBEMAP:
			SAFE(glActiveTexture(GL_TEXTURE0 + material->active_texture_slot));
			SAFE(glBindTexture(GL_TEXTURE_CUBE_MAP, *(GLuint*)prop->value));
			SAFE(glUniform1i(prop->glId, material->active_texture_slot));
			material->active_texture_slot++;
			break;
		default:
			throw CrossException("Unknown property type(%s)", prop->name.c_str());
		}
	}
	material->active_texture_slot = 0;

	if(shader->UseLights()) {
		shader->TransferLightData(scene->GetLights());
	}

	//binding attributes
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VertexBuffer* vertexBuf = vertex_buffer;
	U32 vertexSize = vertexBuf->VertexSize();
	if(shader->aPosition != -1) {
		SAFE(glEnableVertexAttribArray(shader->aPosition));
		SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)(0 + vertexBuf->GetPossitionsOffset())));
	}
	if(shader->aTexCoords != -1) {
		if(vertexBuf->HasTextureCoordinates()) {
			SAFE(glEnableVertexAttribArray(shader->aTexCoords));
			SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetTextureCoordinatesOffset()));
		} else {
			throw CrossException("Current mesh does not contain texture coordinates");
		}
	}
	if(shader->aNormal != -1) {
		if(vertexBuf->HasNormals()) {
			SAFE(glEnableVertexAttribArray(shader->aNormal));
			SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetNormalsOffset()));
		} else {
			throw CrossException("Current mesh does not countain normals");
		}
	}
	if(shader->aTangent != -1) {
		if(vertexBuf->HasTangents()) {
			SAFE(glEnableVertexAttribArray(shader->aTangent));
			SAFE(glVertexAttribPointer(shader->aTangent, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetTangentsOffset()));
		} else {
			throw CrossException("Current mesh does not contain tangents");
		}
	}
	if(shader->aBitangent != -1) {
		if(vertexBuf->HasBitangents()) {
			SAFE(glEnableVertexAttribArray(shader->aBitangent));
			SAFE(glVertexAttribPointer(shader->aBitangent, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetBitangentsOffset()));
		} else {
			throw CrossException("Current mesh does not contain bitangents");
		}
	}

	//drawing
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	//depth test
	SAFE(glEnable(GL_DEPTH_TEST));
	//face culling
	if(IsFaceCullingEnabled()) {
		SAFE(glEnable(GL_CULL_FACE));
	}
	//stencil test
	switch(stencilBehvaiour) {
	case Graphics3D::StencilBehaviour::WRITE:
		SAFE(glEnable(GL_STENCIL_TEST));
		SAFE(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
		SAFE(glStencilFunc(GL_ALWAYS, 1, 0xFF));
		break;
	case Graphics3D::StencilBehaviour::READ:
		SAFE(glEnable(GL_STENCIL_TEST));
		SAFE(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
		SAFE(glStencilMask(0xFF));
		break;
	case Graphics3D::StencilBehaviour::IGNORED:
		break;
	default:
		throw CrossException("Unknow stecil behaviour");
	}
	//alpha blending
	if(material->transparency) {
		SAFE(glEnable(GL_BLEND));
		SAFE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	SAFE(glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_BLEND));
	SAFE(glDisable(GL_STENCIL_TEST));
	SAFE(glDisable(GL_CULL_FACE));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Mesh::TransferVideoData() {
	index_count = indices.size();

	SAFE(glGenBuffers(1, &VBO));
	SAFE(glGenBuffers(1, &EBO));

	SAFE(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	SAFE(glBufferData(GL_ARRAY_BUFFER, vertex_buffer->GetDataSize(), vertex_buffer->GetData(), GL_STATIC_DRAW));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	SAFE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	vertex_buffer->Free();
	initialized = true;
}

void Mesh::PushData(VertexBuffer* buffer, const Array<GLushort>& inds) {
	if(vertex_buffer == NULL) {
		vertex_buffer = buffer->Clone();
	} else {
		vertex_buffer->PushData(buffer->GetData(), buffer->GetDataSize());
	}

	U32 indsOffset = indices.size();
	for(U32 i = 0; i < inds.size(); ++i) {
		indices.push_back(indsOffset + inds[i]);
	}
}

void Mesh::SetMaterial(Material* mat) {
	this->material = mat;
}

Material* Mesh::GetMaterial() {
	return material;
}

void Mesh::SetFaceCullingEnabled(bool enabled){
	face_culling = enabled;
}

bool Mesh::IsFaceCullingEnabled() const{
	return face_culling;
}

VertexBuffer* Mesh::GetVertexBuffer() {
	return vertex_buffer;
}

Array<GLushort>& Mesh::GetIndices() {
	return indices;
}

Mesh* Mesh::Clone() const{
	return new Mesh(*this);
}
