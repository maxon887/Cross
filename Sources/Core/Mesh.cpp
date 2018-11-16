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
#include "Mesh.h"
#include "System.h"
#include "VertexBuffer.h"
#include "Scene.h"
#include "Texture.h"
#include "Entity.h"
#include "Material.h"
#include "Game.h"
#include "Camera.h"
#include "Transform.h"
#include "Utils/Cubemap.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Mesh::Mesh() : Component("Mesh")
{ }

Mesh::Mesh(const String& modelfilename, S32 id) : 
	Component("Mesh")
{
	this->id = id;
	this->model_filename = modelfilename;
}

Mesh::~Mesh() {
	delete vertex_buffer;
	if(original && initialized) {
		SAFE(glDeleteBuffers(1, (GLuint*)&VBO));
		SAFE(glDeleteBuffers(1, (GLuint*)&EBO));
	}
}

void Mesh::Initialize(Scene* scene) {
	if(!model_filename.value.IsEmpty()) {
		CROSS_FAIL(id != -1, "Can not initialize Mesh with ID = -1");
		Model* model = scene->GetModel(model_filename);
		Copy(model->GetMesh(id));

		if(material_filename.value != "") {
			Material* mat = scene->GetMaterial(material_filename);
			SetMaterial(mat);
		} else {
			SetMaterial(scene->GetMaterial("Engine/Default.mat"));
		}
	}
}

void Mesh::Update(float sec) {
	Draw();
}

Mesh* Mesh::Clone() const {
	Mesh* mesh = new Mesh();
	mesh->Copy(this);
	return mesh;
}

void Mesh::Draw() {
	Draw(material);
}

void Mesh::Draw(Material* mat) {
	Draw(mat, StencilBehaviour::IGNORED);
}

void Mesh::Draw(Material* mat, StencilBehaviour sten) {
	if(GetEntity()) {
		CROSS_FAIL(GetEntity()->GetComponent<Transform>(), "Can not draw mesh without transform");
		Draw(GetEntity()->GetWorldMatrix(), mat, sten);
	} else {
		Draw(Matrix::Identity, mat, sten);
	}
}

void Mesh::Draw(const Matrix& globalModel, Material* material,
				StencilBehaviour stencilBehaviour) {
	CROSS_FAIL(initialized, "Attempt to draw with not initialized mesh");
	CROSS_FAIL(material, "Attempt to draw without material");
	Shader* shader = material->GetShader();
	shader->Use();

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

	if(shader->uAmbientLight != -1) {
		SAFE(glUniform4fv(shader->uAmbientLight, 1, scene->GetAmbientColor().GetData()));
	}

	for(Shader::Property& prop : material->GetProperties()) {
		if(prop.glId == -1) {
			//late shader compilation produce this, trying to assign compiled id to the material id
			Shader::Property* shaderProp = shader->GetProperty(prop.GetName());
			prop.glId = shaderProp->GetID();
			CROSS_FAIL(prop.glId != -1, "Broken shader property");
		}

		switch(prop.type) {
		case Shader::Property::TEXTURE:
			SAFE(glActiveTexture(GL_TEXTURE0 + material->active_texture_slot));
			SAFE(glBindTexture(GL_TEXTURE_2D, (GLuint)prop.value.texture->GetID()));
			SAFE(glUniform1i(prop.glId, material->active_texture_slot));
			material->active_texture_slot++;
			break;
		case Shader::Property::MAT4:
			SAFE(glUniformMatrix4fv(prop.glId, 1, GL_FALSE, prop.value.mat.GetData()));
			break;
		case Shader::Property::COLOR:
			SAFE(glUniform4fv(prop.glId, 1, prop.value.color.GetData()));
			break;
		case Shader::Property::VEC3:
			SAFE(glUniform3fv(prop.glId, 1, prop.value.vec3.GetData()));
			break;
		case Shader::Property::FLOAT:
			SAFE(glUniform1f(prop.glId, prop.value.f));
			break;
		case Shader::Property::INT:
			SAFE(glUniform1i(prop.glId, prop.value.s32));
			break;
		case Shader::Property::CUBEMAP:
			SAFE(glActiveTexture(GL_TEXTURE0 + material->active_texture_slot));
			SAFE(glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)prop.value.cubemap->GetTextureID()));
			SAFE(glUniform1i(prop.glId, material->active_texture_slot));
			material->active_texture_slot++;
			break;
		default:
			CROSS_ASSERT(false, "Unknown property type(#)", prop.GetName());
		}
	}
	material->active_texture_slot = 0;

	shader->OnDraw();

	//binding attributes
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, (GLuint)VBO));
	U32 vertexSize = vertex_buffer->VertexSize();
	if(shader->aPosition != -1) {
		SAFE(glEnableVertexAttribArray((GLuint)shader->aPosition));
		SAFE(glVertexAttribPointer((GLuint)shader->aPosition, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertex_buffer->GetPositionsOffset()));
	}
	if(shader->aTexCoords != -1) {
		CROSS_FAIL(vertex_buffer->HasTextureCoordinates(), "Current mesh does not contain texture coordinates");
		SAFE(glEnableVertexAttribArray((GLuint)shader->aTexCoords));
		SAFE(glVertexAttribPointer((GLuint)shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertex_buffer->GetTextureCoordinatesOffset()));
	}
	if(shader->aNormal != -1) {
		CROSS_FAIL(vertex_buffer->HasNormals(), "Current mesh does not countain normals");
		SAFE(glEnableVertexAttribArray((GLuint)shader->aNormal));
		SAFE(glVertexAttribPointer((GLuint)shader->aNormal, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertex_buffer->GetNormalsOffset()));
	}
	if(shader->aTangent != -1) {
		CROSS_FAIL(vertex_buffer->HasTangents(), "Current mesh does not contain tangents");
		SAFE(glEnableVertexAttribArray((GLuint)shader->aTangent));
		SAFE(glVertexAttribPointer((GLuint)shader->aTangent, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertex_buffer->GetTangentsOffset()));
	}
	if(shader->aBitangent != -1) {
		CROSS_FAIL(vertex_buffer->HasBitangents(), "Current mesh does not contain bitangents");
		SAFE(glEnableVertexAttribArray((GLuint)shader->aBitangent));
		SAFE(glVertexAttribPointer((GLuint)shader->aBitangent, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertex_buffer->GetBitangentsOffset()));
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
	switch(stencilBehaviour) {
	case StencilBehaviour::WRITE:
		SAFE(glEnable(GL_STENCIL_TEST));
		SAFE(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
		SAFE(glStencilFunc(GL_ALWAYS, 1, 0xFF));
		break;
	case StencilBehaviour::READ:
		SAFE(glEnable(GL_STENCIL_TEST));
		SAFE(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
		SAFE(glStencilMask(0xFF));
		break;
	case StencilBehaviour::IGNORED:
		break;
	default:
		CROSS_ASSERT(false, "Unknown stencil behaviour");
	}
	//alpha blending
	if(material->IsTransparent()) {
		SAFE(glEnable(GL_BLEND));
		SAFE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)EBO));
	SAFE(glDrawElements(GL_TRIANGLES, (S32)indices.Size(), GL_UNSIGNED_SHORT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_BLEND));
	SAFE(glDisable(GL_STENCIL_TEST));
	SAFE(glDisable(GL_CULL_FACE));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Mesh::TransferVideoData() {
	SAFE(glGenBuffers(1, (GLuint*)&VBO));
	SAFE(glGenBuffers(1, (GLuint*)&EBO));

	SAFE(glBindBuffer(GL_ARRAY_BUFFER, (GLuint)VBO));
	SAFE(glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vertex_buffer->GetDataSize(), vertex_buffer->GetData(), GL_STATIC_DRAW));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)EBO));
	SAFE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)indices.Size() * sizeof(GLushort), indices.GetData(), GL_STATIC_DRAW));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	vertex_buffer->Free();
	initialized = true;
}

void Mesh::PushData(VertexBuffer* buffer, const Array<GLushort>& inds) {
	if(vertex_buffer == nullptr) {
		vertex_buffer = buffer->Clone();
	} else {
		vertex_buffer->PushData(buffer->GetData(), buffer->GetDataSize());
	}

	U16 indsOffset = (U16)indices.Size();
	for(S32 i = 0; i < inds.Size(); ++i) {
		indices.Add(indsOffset + inds[i]);
	}
}

void Mesh::SetMaterial(Material* mat) {
	this->material = mat;
	material_filename = mat->GetFilename();
}

Material* Mesh::GetMaterial() {
	return material;
}

bool Mesh::IsFaceCullingEnabled() const {
	return face_culling;
}

void Mesh::EnableFaceCulling(bool yes) {
	face_culling = yes;
}

VertexBuffer* Mesh::GetVertexBuffer() {
	return vertex_buffer;
}

Array<GLushort>& Mesh::GetIndices() {
	return indices;
}

S32 Mesh::GetID() const {
	return id;
}

U32 Mesh::GetPolyCount() const {
	return (U32)indices.Size();
}

bool Mesh::IsEqual(Mesh* other) const {
	return this->VBO == other->VBO && this->EBO == other->EBO;
}

void Mesh::Copy(const Mesh* m) {
	VBO = m->VBO;
	EBO = m->EBO;
	vertex_buffer = m->vertex_buffer;
	id = m->id;
	model_filename = m->model_filename;
	material = m->material;
	indices = m->indices;
	initialized = m->initialized;
	face_culling = m->face_culling;
	original = false;

	vertex_buffer = m->vertex_buffer->Clone();
}
