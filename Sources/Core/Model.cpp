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

using namespace cross;

Model::~Model() {
	delete hierarchy;
	for(std::pair<String, Mesh*> p : meshes) {
		delete p.second;
	}
}

bool Model::Load(const String& filename) {
	return Load(filename, false);
}

bool Model::Load(const String& filename, bool calcTangents) {
	return Load(filename, calcTangents, true);
}

bool Model::Load(const String& inFilename, bool calcTangents, bool initializeVideoData) {
	CROSS_RETURN(os->IsAssetFileExists(inFilename), false, "Model file doesn't exists:\n:#", inFilename);
	Debugger::Instance()->SetTimeCheck();
	filename = inFilename;
	initialize_video = initializeVideoData;
	Entity* root = CREATE Entity("ModelRoot");
	hierarchy = root;
	File* file = os->LoadAssetFile(filename);
	CROSS_RETURN(file, false, "Can not load model file: #", inFilename);
	bool result = ProcessScene(root, file, calcTangents, filename);
	delete file;

	float loadTime = Debugger::Instance()->GetTimeCheck();
	os->LogIt("Model(#) loaded in #ms", filename, String(loadTime, "%0.1f", 12));

	return result;
}

Entity* Model::GetHierarchy() const {
	return hierarchy->Clone();
}

Mesh* Model::GetMesh(const String& id) {
	if(meshes.find(id) != meshes.end()) {
		return meshes[id];
	} else {
		return nullptr;
	}
}

const String& Model::GetName() const {
	return filename;
}

bool Model::ProcessScene(Entity* root, File* file, bool calcTangents, const String& filename) {
	Assimp::Importer importer;

	unsigned int flags = aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_MakeLeftHanded;
	if(calcTangents) {
		flags |= aiProcess_CalcTangentSpace;
	}
	current_scene = importer.ReadFileFromMemory(file->data, (Size)file->size, flags);
	if(!current_scene || current_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !current_scene->mRootNode) {
		CROSS_RETURN(false, false, "Assimp Error: #", importer.GetErrorString());
	}

	aiNode* aiRoot = current_scene->mRootNode;
	if(aiRoot->mNumChildren == 1) {
		root->SetName(aiRoot->mChildren[0]->mName.C_Str());
		ProcessNode(root, aiRoot->mChildren[0], filename);
	} else {
		root->SetName(File::FileFromPath(File::FileWithoutExtension(filename)));
		ProcessNode(root, aiRoot, filename);
	}
	return true;
}

void Model::ProcessNode(Entity* entity, aiNode* node, const String& filename) {
	Matrix modelMat = Matrix::Zero;
	memcpy(modelMat.m, &node->mTransformation.a1, sizeof(float) * 16);
	Transform* transform = CREATE Transform();
	transform->SetPosition(modelMat.GetTranslation());
	transform->SetScale(modelMat.GetScale());
	transform->SetRotate(modelMat.GetRotation());
	entity->AddComponent(transform);

	if(node->mNumMeshes) {
		aiMesh* aiMesh = current_scene->mMeshes[node->mMeshes[0]];
		Mesh* crMesh = ProcessMesh(aiMesh, filename, entity->GetName());
		entity->AddComponent(crMesh);
	}

	for(U32 i = 0; i < node->mNumChildren; ++i) {
		Entity* child = CREATE Entity(node->mChildren[i]->mName.C_Str());
		child->SetParent(entity);
		ProcessNode(child, node->mChildren[i], filename);
		entity->AddChild(child);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const String& filename, const String& groupID) {
	VertexBuffer* vertexBuffer = CREATE VertexBuffer();
	if(mesh->mTextureCoords[0]) {
		vertexBuffer->UVEnabled(true);
	}
	if(mesh->mNormals) {
		vertexBuffer->NormalsEnabled(true);
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
			indices.Add((U16)mesh->mFaces[i].mIndices[j]);
		}
	}
	//os->LogIt("\tMesh loaded with # polygons and # bytes consumed", mesh->mNumFaces, vertexBuffer->GetDataSize());
	//we need to create 2 Meshes he one is for video data storage second is for hierarchy
	Mesh* videoMesh = CREATE Mesh(filename, groupID);
	videoMesh->PushData(vertexBuffer, indices);
	delete vertexBuffer;
	if(initialize_video) {
		videoMesh->InitializeVideoData();
	}
	CROSS_ASSERT(meshes.find(groupID) == meshes.end(), "Model already contains GroupID '#'", groupID);
	meshes[groupID] = videoMesh;
	
	Mesh* hierarchyMesh = CREATE Mesh(filename, groupID);
	return hierarchyMesh;
}
