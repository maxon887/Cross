/*	Copyright � 2015 Lukyanau Maksim

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
#include "Graphics3D.h"
#include "GraphicsGL.h"
#include "VertexBuffer.h"
#include "System.h"
#include "Mesh.h"
#include "File.h"
#include "Model.h"
#include "Internals/Debugger.h"
#include "Scene.h"
#include "Light.h"
#include "Game.h"
#include "Camera.h"
#include "Shaders/Shader.h"
#include "Material.h"
#include "Entity.h"

#include <algorithm>

#define SWIG

#include "Libs/Assimp/Importer.hpp"
#include "Libs/Assimp/scene.h"
#include "Libs/Assimp/postprocess.h"
#include "Libs/Assimp/version.h"

using namespace cross;

Graphics3D::Graphics3D():
	current_translation(Matrix::Identity),
	current_pre_rotation(Matrix::Identity),
	current_rotation(Matrix::Identity),
	current_scaling(Matrix::Identity),
	current_geotranslation(Matrix::Identity),
	initialize_in_load(true)
{
	system->LogIt("Graphics3D::Graphics3D()");
	U32 major = aiGetVersionMajor();
	U32 minor = aiGetVersionMinor();
	system->LogIt("\tUse assimp version %d.%d", major, minor);
	memset(primitives, 0, sizeof(primitives));
}

Graphics3D::~Graphics3D(){
	for(U32 i = 0; i < Primitives::COUNT; ++i){
		delete primitives[i];
	}
}

Entity* Graphics3D::LoadPrimitive(Graphics3D::Primitives primitive, bool initialize){
	if(primitives[primitive]){
		Entity* prim = primitives[primitive]->Clone();
		/*
		if(initialize){
			Mesh* mesh = (Mesh*)prim->GetComponent(Component::Type::MESH);
			if(mesh){
				mesh->TransferVideoData();
			}else{
				throw CrossException("Primitive entity does not contain Mesh");
			}
		}*/
		return prim;
	}else{
		switch(primitive) {
		case cross::Graphics3D::CUBE:
			primitives[primitive] = LoadModel("Engine/Models/Cube.obj", true);
			break;
		case cross::Graphics3D::SPHERE:
			primitives[primitive] = LoadModel("Engine/Models/Sphere.obj", true);
			break;
		case cross::Graphics3D::PLANE:
			primitives[primitive] = LoadModel("Engine/Models/Plane.obj", true);
			break;
		default:
			throw CrossException("Unknown primitive type");
		}
		return LoadPrimitive(primitive, initialize);
	}
}

Entity* Graphics3D::LoadModel(const string& filename, bool initialize){
	initialize_in_load = initialize;
	Debugger::Instance()->SetTimeCheck();
	Entity* model = new Entity();
	ProcessScene(model, filename);
	float loadTime = Debugger::Instance()->GetTimeCheck();
	system->LogIt("Model(%s) loaded in %0.1fms", filename.c_str(), loadTime);
	return model;
}

void Graphics3D::AdjustMaterial(Entity* model, Material* material, bool faceCulling, bool alphaBlending){
	Mesh* mesh = (Mesh*)model->GetComponent(Component::Type::MESH);
	if(mesh){
		mesh->SetMaterial(material);
		mesh->SetFaceCullingEnabled(faceCulling);
		mesh->SetAlphaBlendingEnabled(alphaBlending);
	}
	for(Entity* child : model->GetChildren()){
		AdjustMaterial(child, material);
	}
}

void Graphics3D::DrawMesh(Mesh* mesh, const Matrix& globalModel){
	DrawMesh(mesh, globalModel, StencilBehaviour::IGNORED);
}

void Graphics3D::DrawMesh(Mesh* mesh, const Matrix& globalModel, StencilBehaviour stencilBehvaiour){
	if(!mesh->initialized){
		throw CrossException("Before draw mesh needs to be initialized");
	}
	Material* material = mesh->GetMaterial();
	if(material == nullptr){
		throw CrossException("Current mesh does not have material");
	}
	Shader* shader = material->GetShader();
	gfxGL->UseShader(shader);

	Scene* scene = game->GetCurrentScene();
	//binding uniforms
	if(shader->uMVP != -1){
		Camera* cam = scene->GetCamera();
		Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix() * globalModel;
		mvp = mvp.GetTransposed();
		SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	}

	if(shader->uModelMatrix != -1){
		Matrix model = globalModel;
		model = model.GetTransposed();
		SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.GetData()));
	}

	if(shader->uNormalMatrix != -1){
		Matrix normal = (globalModel).GetInversed();
		SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normal.GetData()));
	}

	if(shader->uCameraPosition != -1){
		SAFE(glUniform3fv(shader->uCameraPosition, 1, scene->GetCamera()->GetPosition().GetData()));
	}

	if(shader->uColor != -1){
		SAFE(glUniform4fv(shader->uColor, 1, Color::White.GetData()));
	}

	if(shader->uAmbientLight != -1){
		SAFE(glUniform4fv(shader->uAmbientLight, 1, scene->GetAmbientColor().GetData()));
	}

	for(Shader::Property* prop : material->properties){
		if(prop->glId == -1){
			throw CrossException("Broken shader property");
		}
		if(prop->value == NULL){
			throw CrossException("Property '%s' value not assigned", prop->name.c_str());
		}

		switch(prop->type){
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
			throw CrossException("Unknown property type");
		}
	}
	material->active_texture_slot = 0;

	if(shader->UseLights()){
		shader->TransferLightData(scene->GetLights());
	}

	//binding attributes
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO));
	VertexBuffer* vertexBuf = mesh->vertex_buffer;
	U32 vertexSize = vertexBuf->VertexSize();
	if(shader->aPosition != -1){
		SAFE(glEnableVertexAttribArray(shader->aPosition));
		SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)(0 + vertexBuf->GetPossitionsOffset())));
	}
	if(shader->aTexCoords != -1){
		if(vertexBuf->HasTextureCoordinates()){
			SAFE(glEnableVertexAttribArray(shader->aTexCoords));
			SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetTextureCoordinatesOffset()));
		}else{
			throw CrossException("Current mesh does not contain texture coordinates");
		}
	}
	if(shader->aNormal != -1){
		if(vertexBuf->HasNormals()){
			SAFE(glEnableVertexAttribArray(shader->aNormal));
			SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetNormalsOffset()));
		}else{
			throw CrossException("Current mesh does not countain normals");
		}
	}
	if(shader->aTangent != -1){
		if(vertexBuf->HasTangents()){
			SAFE(glEnableVertexAttribArray(shader->aTangent));
			SAFE(glVertexAttribPointer(shader->aTangent, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetTangentsOffset()));
		}else{
			throw CrossException("Current mesh does not contain tangents");
		}
	}
	if(shader->aBitangent != -1){
		if(vertexBuf->HasBitangents()){
			SAFE(glEnableVertexAttribArray(shader->aBitangent));
			SAFE(glVertexAttribPointer(shader->aBitangent, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLfloat*)0 + vertexBuf->GetBitangentsOffset()));
		}else{
			throw CrossException("Current mesh does not contain bitangents");
		}
	}

	//drawing
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	//depth test
	SAFE(glEnable(GL_DEPTH_TEST));
	//face culling
	if(mesh->IsFaceCullingEnabled()){
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
		throw CrossException("Unknow stecil behaviour");
	}
	//alpha blending
	if(mesh->IsAlphaBlendingEnabled()){
		SAFE(glEnable(GL_BLEND));
		SAFE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_SHORT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_BLEND));
	SAFE(glDisable(GL_STENCIL_TEST));
	SAFE(glDisable(GL_CULL_FACE));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::ProcessScene(Entity* model, const string& filename){
	File* file = system->LoadFile(filename);
	string extension = system->ExtensionFromFile(filename);
	if(extension == "fbx" || extension == "FBX") {
		format = FBX;
	} else {
		format = UNKNOW;
	}
	Assimp::Importer importer;
	current_scene = importer.ReadFileFromMemory(file->data, file->size, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	delete file;
	if(!current_scene || current_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !current_scene->mRootNode){
		throw CrossException("Assimp Error: %s", importer.GetErrorString());
	}
	ProcessNode(model, current_scene->mRootNode);
}

void Graphics3D::ProcessNode(Entity* model, aiNode* node){
	for(U32 i = 0; i < node->mNumMeshes; i++){
		aiMesh* aiMesh = current_scene->mMeshes[node->mMeshes[i]];
		Mesh* crMesh = ProcessMesh(aiMesh);
		crMesh->SetName(node->mName.C_Str());
		if(format == Model::Format::FBX){
			model->SetModelMatrix(current_translation * current_pre_rotation * current_rotation * current_scaling * current_geotranslation);
			current_translation = Matrix::Identity;
			current_pre_rotation = Matrix::Identity;
			current_rotation = Matrix::Identity;
			current_scaling = Matrix::Identity;
			current_geotranslation = Matrix::Identity;
		}else{
			Matrix modelMat = Matrix::Zero;
			memcpy(modelMat.m, &node->mTransformation.a1, sizeof(float) * 16);
			model->SetModelMatrix(modelMat);
		}
		model->AddComponent(crMesh);
	}
	if(format == Model::Format::FBX){
		string nodeName = node->mName.C_Str();
		if(nodeName.find("Translation") != std::string::npos){
			if(nodeName.find("Geometric") != std::string::npos){
				memcpy(current_geotranslation.m, &node->mTransformation.a1, sizeof(float) * 16);
			}else{
				if(current_translation == Matrix::Identity){
					memcpy(current_translation.m, &node->mTransformation.a1, sizeof(float) * 16);
				}
			}
		}
		if(nodeName.find("Scaling") != std::string::npos){
			memcpy(current_scaling.m, &node->mTransformation.a1, sizeof(float) * 16);
		}
		if(nodeName.find("Rotation") != std::string::npos){
			if(nodeName.find("Pre") != std::string::npos){
				memcpy(current_pre_rotation.m, &node->mTransformation.a1, sizeof(float) * 16);
			}else{
				if(current_translation == Matrix::Identity){
					memcpy(current_translation.m, &node->mTransformation.a1, sizeof(float) * 16);
				}
			}
		}
	}
	for(U32 i = 0; i < node->mNumChildren; ++i){
		Entity* child = new Entity();
		child->SetParent(model);
		ProcessNode(child, node->mChildren[i]);
		model->AddChild(child);
	}
}

Mesh* Graphics3D::ProcessMesh(aiMesh* mesh){
	VertexBuffer* vertexBuffer = new VertexBuffer();
	if(mesh->mTextureCoords[0]){
		vertexBuffer->UVEnabled(true);
	}
	if(mesh->mNormals){
		vertexBuffer->NarmalsEnabled(true);
	}
	if(mesh->mTangents){
		vertexBuffer->TangentsEnabled(true);
	}
	if(mesh->mBitangents){
		vertexBuffer->BitangentsEnabled(true);
	}

	for(U32 i = 0; i < mesh->mNumVertices; ++i){
		vertexBuffer->PushData((Byte*)&mesh->mVertices[i], 3 * sizeof(float));

		if(vertexBuffer->HasTextureCoordinates()){
			vertexBuffer->PushData((Byte*)&mesh->mTextureCoords[0][i], 2 * sizeof(float));
		}

		if(vertexBuffer->HasNormals()){
			vertexBuffer->PushData((Byte*)&mesh->mNormals[i], 3 * sizeof(float));
		}

		if(vertexBuffer->HasTangents()){
			vertexBuffer->PushData((Byte*)&mesh->mTangents[i], 3 * sizeof(float));
		}

		if(vertexBuffer->HasBitangents()){
			vertexBuffer->PushData((Byte*)&mesh->mBitangents[i], 3 * sizeof(float));
		}
	}

	Array<GLushort> indices;
	for(U32 i = 0; i < mesh->mNumFaces; ++i){
		for(U32 j = 0; j < mesh->mFaces[i].mNumIndices; ++j){
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
	cross::system->LogIt("Mesh loaded with %d polygons", mesh->mNumFaces);
	Mesh* crsMesh = new Mesh();
	crsMesh->PushData(vertexBuffer, indices);
	delete vertexBuffer;
	if(initialize_in_load){
		crsMesh->TransferVideoData();
	}
	return crsMesh;
}