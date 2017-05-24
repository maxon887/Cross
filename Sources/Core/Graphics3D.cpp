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
	initialize_in_load(true)
{
	sys->LogIt("Graphics3D::Graphics3D()");
	U32 major = aiGetVersionMajor();
	U32 minor = aiGetVersionMinor();
	sys->LogIt("\tUse assimp version %d.%d", major, minor);
	memset(primitives, 0, sizeof(primitives));
	simple_shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	simple_shader->Compile();
}

Graphics3D::~Graphics3D(){
	for(U32 i = 0; i < Primitives::COUNT; ++i){
		if(primitives[i]){
			primitives[i]->DeleteChildren();
			delete primitives[i];
		}
	}
	delete simple_shader;
}

void Graphics3D::DrawLine(const Vector3D& p1, const Vector3D& p2, const Color& c) {
	gfxGL->UseShader(simple_shader);
	float vertices[6] = { p1.x, p1.y, p1.z, p2.x, p2.y, p2.z };
	Camera* cam = game->GetCurrentScene()->GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.GetTransposed();
	SAFE(glUniformMatrix4fv(simple_shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glVertexAttribPointer(simple_shader->aPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices));
	SAFE(glUniform4fv(simple_shader->uColor, 1, c.GetData()));
	SAFE(glEnableVertexAttribArray(simple_shader->aPosition));
	SAFE(glDrawArrays(GL_LINES, 0, 2));
}

Entity* Graphics3D::LoadPrimitive(Graphics3D::Primitives primitive){
	if(primitives[primitive]){
		return primitives[primitive]->Clone();
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
		return LoadPrimitive(primitive);
	}
}

Entity* Graphics3D::LoadModel(const string& filename, bool initialize){
	initialize_in_load = initialize;
	Debugger::Instance()->SetTimeCheck();
	Entity* model = new Entity();
	ProcessScene(model, filename);
	float loadTime = Debugger::Instance()->GetTimeCheck();
	sys->LogIt("Model(%s) loaded in %0.1fms", filename.c_str(), loadTime);
	return model;
}

void Graphics3D::AdjustMaterial(Entity* model, Material* material, bool faceCulling){
	if(model->HasComponent(Component::Type::MESH)) {
		Mesh* mesh = (Mesh*)model->GetComponent(Component::Type::MESH);
		mesh->SetMaterial(material);
		mesh->SetFaceCullingEnabled(faceCulling);
	}
	for(Entity* child : model->GetChildren()){
		AdjustMaterial(child, material);
	}
}

void Graphics3D::ProcessScene(Entity* model, const string& filename){
	File* file = sys->LoadFile(filename);
	Assimp::Importer importer;
	current_scene = importer.ReadFileFromMemory(file->data, file->size, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	delete file;
	if(!current_scene || current_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !current_scene->mRootNode){
		throw CrossException("Assimp Error: %s", importer.GetErrorString());
	}
	ProcessNode(model, current_scene->mRootNode);
}

void Graphics3D::ProcessNode(Entity* entity, aiNode* node){
	entity->SetName(node->mName.C_Str());

	Matrix modelMat = Matrix::Zero;
	memcpy(modelMat.m, &node->mTransformation.a1, sizeof(float) * 16);
	entity->SetTranslate(modelMat.GetTranslation());
	entity->SetRotate(modelMat.GetRotation());
	//entity->SetModelMatrix(modelMat);

	if(node->mNumMeshes) {
		aiMesh* aiMesh = current_scene->mMeshes[node->mMeshes[0]];
		Mesh* crMesh = ProcessMesh(aiMesh);
		entity->AddComponent(crMesh);
	}

	for(U32 i = 0; i < node->mNumChildren; ++i) {
		Entity* child = new Entity();
		child->SetParent(entity);
		ProcessNode(child, node->mChildren[i]);
		entity->AddChild(child);
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
	sys->LogIt("Mesh loaded with %d polygons", mesh->mNumFaces);
	Mesh* crsMesh = new Mesh();
	crsMesh->PushData(vertexBuffer, indices);
	delete vertexBuffer;
	if(initialize_in_load){
		crsMesh->TransferVideoData();
	}
	return crsMesh;
}