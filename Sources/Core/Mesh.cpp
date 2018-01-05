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
#include "System.h"
#include "VertexBuffer.h"
#include "Scene.h"
#include "Texture.h"
#include "Entity.h"
#include "Material.h"
#include "Game.h"
#include "Camera.h"
#include "Transform.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Mesh::Mesh(Model* model, S32 id) :
	model(model),
	id(id)
{ }

Mesh::~Mesh() {
	delete vertex_buffer;
	if(original && initialized) {
		SAFE(glDeleteBuffers(1, (GLuint*)&VBO));
		SAFE(glDeleteBuffers(1, (GLuint*)&EBO));
	}
}

void Mesh::Update(float sec){
	Draw();
}

Mesh* Mesh::Clone() const {
	Mesh* mesh = new Mesh();
	mesh->Copy(this);
	return mesh;
}

bool Mesh::Load(XMLElement* xml, Scene* scene) {
	S32 id = xml->IntAttribute("id", -1);
	CROSS_RETURN(id != -1, false, "Attribute 'id' not found");
	const char* modelfilename = xml->Attribute("model");
	CROSS_RETURN(modelfilename, false, "Attribute 'model' not found");

	Model* model = scene->GetModel(modelfilename);
	Copy(model->GetMesh(id));

	const char* materialFile = xml->Attribute("material");
	if(materialFile) {
		Material* mat = scene->GetMaterial(materialFile);
		SetMaterial(mat);
	} else {
		SetMaterial(scene->GetMaterial("Engine/Default.mat"));
	}
	return true;
}

bool Mesh::Save(XMLElement* xml, XMLDocument* doc) {
	XMLElement* meshXML = doc->NewElement("Mesh");
	meshXML->SetAttribute("id", GetID());
	meshXML->SetAttribute("model", GetModel()->GetFilename().c_str());
	meshXML->SetAttribute("material", GetMaterial()->GetFilename().c_str());
	xml->LinkEndChild(meshXML);
	return true;
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
				StencilBehaviour stencilBehvaiour) {
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

	if(shader->uColor != -1) {
		SAFE(glUniform4fv(shader->uColor, 1, Color::White.GetData()));
	}

	if(shader->uAmbientLight != -1) {
		SAFE(glUniform4fv(shader->uAmbientLight, 1, scene->GetAmbientColor().GetData()));
	}

	for(Shader::Property* prop : material->properties) {
		if(prop->glId == -1) {
			//late shader compilation produce this
			Shader::Property* shaderProp = shader->GetProperty(prop->GetName());
			prop->glId = shaderProp->GetID();
			CROSS_FAIL(prop->glId != -1, "Broken shader property");
		}
		CROSS_FAIL(prop->value, "Property '%s' value does not assigned", prop->name.c_str());

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
		case Shader::Property::COLOR:
			SAFE(glUniform4fv(prop->glId, 1, (GLfloat*)prop->value));
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
			CROSS_ASSERT(false, "Unknown property type(%s)", prop->name.c_str());
		}
	}
	material->active_texture_slot = 0;

	shader->OnDraw();

	//binding attributes
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	VertexBuffer* vertexBuf = vertex_buffer;
	U32 vertexSize = vertexBuf->VertexSize();
	if(shader->aPosition != -1) {
		SAFE(glEnableVertexAttribArray(shader->aPosition));
		SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetPossitionsOffset()));
	}
	if(shader->aTexCoords != -1) {
		CROSS_FAIL(vertexBuf->HasTextureCoordinates(), "Current mesh does not contain texture coordinates");
		SAFE(glEnableVertexAttribArray(shader->aTexCoords));
		SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetTextureCoordinatesOffset()));
	}
	if(shader->aNormal != -1) {
		CROSS_FAIL(vertexBuf->HasNormals(), "Current mesh does not countain normals");
		SAFE(glEnableVertexAttribArray(shader->aNormal));
		SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetNormalsOffset()));
	}
	if(shader->aTangent != -1) {
		CROSS_FAIL(vertexBuf->HasTangents(), "Current mesh does not contain tangents");
		SAFE(glEnableVertexAttribArray(shader->aTangent));
		SAFE(glVertexAttribPointer(shader->aTangent, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetTangentsOffset()));
	}
	if(shader->aBitangent != -1) {
		CROSS_FAIL(vertexBuf->HasBitangents(), "Current mesh does not contain bitangents");
		SAFE(glEnableVertexAttribArray(shader->aBitangent));
		SAFE(glVertexAttribPointer(shader->aBitangent, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetBitangentsOffset()));
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
		CROSS_ASSERT(false, "Unknow stecil behaviour");
	}
	//alpha blending
	if(material->transparency) {
		SAFE(glEnable(GL_BLEND));
		SAFE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	SAFE(glDrawElements(GL_TRIANGLES, (S32)indices.size(), GL_UNSIGNED_SHORT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_BLEND));
	SAFE(glDisable(GL_STENCIL_TEST));
	SAFE(glDisable(GL_CULL_FACE));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Mesh::TransferVideoData() {
	SAFE(glGenBuffers(1, (GLuint*)&VBO));
	SAFE(glGenBuffers(1, (GLuint*)&EBO));

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

	U32 indsOffset = (U32)indices.size();
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

S32 Mesh::GetID() const {
	return id;
}

Model* Mesh::GetModel() {
	return model;
}

U32 Mesh::GetPolyCount() const {
	return (U32)indices.size();
}

bool Mesh::IsEqual(Mesh* other) const {
	return this->VBO == other->VBO && this->EBO == other->EBO;
}

void Mesh::Copy(const Mesh* m) {
	VBO = m->VBO;
	EBO = m->EBO;
	vertex_buffer = m->vertex_buffer;
	id = m->id;
	model = m->model;
	material = m->material;
	indices = m->indices;
	initialized = m->initialized;
	face_culling = m->face_culling;
	original = false;

	vertex_buffer = m->vertex_buffer->Clone();
}
