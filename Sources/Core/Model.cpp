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
#include "Model.h"
#include "Entity.h"
#include "Mesh.h"
#include "System.h"
#include "File.h"
#include "VertexBuffer.h"
#include "Transform.h"
#include "Utils/Debugger.h"

#define SWIG

#include "Libs/Assimp/Importer.hpp"
#include "Libs/Assimp/scene.h"
#include "Libs/Assimp/postprocess.h"
#include "Libs/Assimp/version.h"

using namespace cross;

Model::~Model() {
	delete hierarchy;
}

void Model::Load(const String& filename) {
	Load(filename, false);
}

void Model::Load(const String& filename, bool calcTangents) {
	Load(filename, calcTangents, true);
}

void Model::Load(const String& filename, bool calcTangents, bool transferVideoData) {
	Debugger::Instance()->SetTimeCheck();

	transfer_video = transferVideoData;
	mesh_id = 0;
	this->filename = filename;
	Entity* root = new Entity("ModelRoot");
	hierarchy = root;
	File* file = system->LoadAssetFile(filename);
	CROSS_FAIL(file, "Can not load model file");
	ProcessScene(root, file, calcTangents);
	delete file;

	float loadTime = Debugger::Instance()->GetTimeCheck();
	system->LogIt("Model(%s) loaded in %0.1fms", filename.ToCStr(), loadTime);
}

const String& Model::GetFilename() const {
	return filename;
}

Entity* Model::GetHierarchy() const {
	return hierarchy->Clone();
}

Mesh* Model::GetMesh(S32 id) {
	return meshes[id];
}

void Model::ProcessScene(Entity* root, File* file, bool calcTangents) {
	Assimp::Importer importer;
	unsigned int flags = aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_Triangulate;
	if(calcTangents) {
		flags |= aiProcess_CalcTangentSpace;
	}
	current_scene = importer.ReadFileFromMemory(file->data, file->size, flags);
	if(!current_scene || current_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !current_scene->mRootNode) {
		CROSS_FAIL(false, "Assimp Error: %s", importer.GetErrorString());
	}
	aiNode* aiRoot = current_scene->mRootNode;
	CROSS_FAIL(aiRoot->mNumChildren == 1, "Failed to load model. Unknown number of root childerns");
	root->SetName(aiRoot->mChildren[0]->mName.C_Str());
	ProcessNode(root, aiRoot->mChildren[0]);
}

void Model::ProcessNode(Entity* entity, aiNode* node) {
	Matrix modelMat = Matrix::Zero;
	memcpy(modelMat.m, &node->mTransformation.a1, sizeof(float) * 16);
	Transform* transform = new Transform();
	transform->SetPosition(modelMat.GetTranslation());
	transform->SetScale(modelMat.GetScale());
	transform->SetRotate(modelMat.GetRotation());
	entity->AddComponent(transform);

	if(node->mNumMeshes) {
		aiMesh* aiMesh = current_scene->mMeshes[node->mMeshes[0]];
		Mesh* crMesh = ProcessMesh(aiMesh);
		meshes[mesh_id] = crMesh;
		mesh_id++;
		entity->AddComponent(crMesh);
	}

	for(U32 i = 0; i < node->mNumChildren; ++i) {
		Entity* child = new Entity(node->mChildren[i]->mName.C_Str());
		child->SetParent(entity);
		ProcessNode(child, node->mChildren[i]);
		entity->AddChild(child);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh) {
	VertexBuffer* vertexBuffer = new VertexBuffer();
	if(mesh->mTextureCoords[0]) {
		vertexBuffer->UVEnabled(true);
	}
	if(mesh->mNormals) {
		vertexBuffer->NarmalsEnabled(true);
	}
	if(mesh->mTangents) {
		vertexBuffer->TangentsEnabled(true);
	}
	if(mesh->mBitangents) {
		vertexBuffer->BitangentsEnabled(true);
	}

	for(U32 i = 0; i < mesh->mNumVertices; ++i) {
		vertexBuffer->PushData((Byte*)&mesh->mVertices[i], 3 * sizeof(float));

		if(vertexBuffer->HasTextureCoordinates()) {
			vertexBuffer->PushData((Byte*)&mesh->mTextureCoords[0][i], 2 * sizeof(float));
		}

		if(vertexBuffer->HasNormals()) {
			vertexBuffer->PushData((Byte*)&mesh->mNormals[i], 3 * sizeof(float));
		}

		if(vertexBuffer->HasTangents()) {
			vertexBuffer->PushData((Byte*)&mesh->mTangents[i], 3 * sizeof(float));
		}

		if(vertexBuffer->HasBitangents()) {
			vertexBuffer->PushData((Byte*)&mesh->mBitangents[i], 3 * sizeof(float));
		}
	}

	Array<U16> indices;
	for(U32 i = 0; i < mesh->mNumFaces; ++i) {
		for(U32 j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
	system->LogIt("\tMesh loaded with %d polygons and %d bytes consumed", mesh->mNumFaces, vertexBuffer->GetDataSize());
	Mesh* crsMesh = new Mesh(this, mesh_id);
	crsMesh->PushData(vertexBuffer, indices);
	delete vertexBuffer;
	if(transfer_video) {
		crsMesh->TransferVideoData();
	}
	return crsMesh;
}
