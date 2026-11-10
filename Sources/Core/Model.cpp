#include "Model.h"
#include "Entity.h"
#include "Mesh.h"
#include "System.h"
#include "File.h"
#include "VertexBuffer.h"
#include "Transform.h"
#include "Utils/Debugger.h"

#include "Libs/assimp/Importer.hpp"
#include "Libs/assimp/scene.h"
#include "Libs/assimp/postprocess.h"

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

bool Model::Load(const String& filename, bool calcTangents, bool initializeVideoData) {
	String extension = File::ExtensionFromFile(filename);
	extension.Lowercase();
	return Load(filename, calcTangents, initializeVideoData, extension == "fbx");
}

bool Model::Load(const String& inFilename, bool calcTangents, bool initializeVideoData, bool bakePivotsAndAxes) {
	CROSS_RETURN(os->IsAssetFileExists(inFilename), false, "Model file doesn't exists:\n#", inFilename);
	Debugger::Instance()->SetTimeCheck();
	filename = inFilename;
	initialize_video = initializeVideoData;
	bake_pivots_and_axes = bakePivotsAndAxes;
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
	//FBX pivots must stay preserved (assimp default): CollapseFbxPivotsHelper() reads them from '$AssimpFbx$' nodes
	unsigned int flags = aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_MakeLeftHanded | aiProcess_GlobalScale;
	if(calcTangents) {
		flags |= aiProcess_CalcTangentSpace;
	}
	current_scene = importer.ReadFileFromMemory(file->data, (Size)file->size, flags);
	if(!current_scene || current_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !current_scene->mRootNode) {
		CROSS_RETURN(false, false, "Assimp Error: #", importer.GetErrorString());
	}

	aiNode* aiRoot = current_scene->mRootNode;
	if(aiRoot->mNumChildren == 1) {
		aiMatrix4x4 chainTransform;
		aiVector3D pivot(0.f, 0.f, 0.f);
		aiNode* realNode = CollapseFbxPivotsHelper(aiRoot->mChildren[0], chainTransform, pivot);
		root->SetName(realNode->mName.C_Str());
		ProcessNode(root, realNode, chainTransform, pivot, aiVector3D(0.f, 0.f, 0.f), filename);
	} else {
		root->SetName(File::FileFromPath(File::FileWithoutExtension(filename)));
		ProcessNode(root, aiRoot, aiRoot->mTransformation, aiVector3D(0.f, 0.f, 0.f), aiVector3D(0.f, 0.f, 0.f), filename);
	}
	return true;
}

/*	FBX stores object placement as rotation/scaling pivots. Assimp keeps vertices pre-placed in
	model space and wraps every pivot into a pair of '$AssimpFbx$' helper nodes which cancel each
	other out, while scene editors like Unity re-bake vertices around the pivot and keep the pivot
	in the object transform. Scenes exported from such editors reference mesh groups by name and
	apply their own transforms on top, so with bake_pivots_and_axes the model is converted to that layout:
	helper chains are collapsed into one node, the pivot is moved from the vertices into the node
	transform, and everything is conjugated by a 180 degree Y rotation because those editors convert
	FBX handedness by mirroring X while aiProcess_MakeLeftHanded mirrors Z.
	This keeps 'node * vertices' intact, so models rendered through GetHierarchy() do not move */
aiNode* Model::CollapseFbxPivotsHelper(aiNode* node, aiMatrix4x4& chainTransform, aiVector3D& pivot) {
	String pivotSuffix = "_$AssimpFbx$_RotationPivot";
	String nodeName = node->mName.C_Str();
	while(bake_pivots_and_axes && nodeName.Find("_$AssimpFbx$_") != -1 && node->mNumChildren == 1) {
		chainTransform = chainTransform * node->mTransformation;
		if(nodeName.Length() >= pivotSuffix.Length() &&
		   nodeName.SubString(nodeName.Length() - pivotSuffix.Length(), nodeName.Length()) == pivotSuffix) {
			pivot.x = node->mTransformation.a4;
			pivot.y = node->mTransformation.b4;
			pivot.z = node->mTransformation.c4;
		}
		node = node->mChildren[0];
		nodeName = node->mName.C_Str();
	}
	chainTransform = chainTransform * node->mTransformation;
	return node;
}

void Model::ProcessNode(Entity* entity, aiNode* node, const aiMatrix4x4& chainTransform, const aiVector3D& pivot, const aiVector3D& parentPivot, const String& filename) {
	aiMatrix4x4 entityTransform = chainTransform;
	if(bake_pivots_and_axes) {
		aiMatrix4x4 fromParentPivot;
		aiMatrix4x4::Translation(aiVector3D(-parentPivot.x, -parentPivot.y, -parentPivot.z), fromParentPivot);
		aiMatrix4x4 toPivot;
		aiMatrix4x4::Translation(pivot, toPivot);
		aiMatrix4x4 mirror;
		aiMatrix4x4::Scaling(aiVector3D(-1.f, 1.f, -1.f), mirror);
		entityTransform = mirror * fromParentPivot * chainTransform * toPivot * mirror;
	}
	Matrix modelMat = Matrix::Zero;
	memcpy(modelMat.m, &entityTransform.a1, sizeof(float) * 16);
	Transform* transform = CREATE Transform();
	transform->SetPosition(modelMat.GetTranslation());
	transform->SetScale(modelMat.GetScale());
	transform->SetRotate(modelMat.GetRotation());
	entity->AddComponent(transform);

	if(node->mNumMeshes) {
		aiMesh* aiMesh = current_scene->mMeshes[node->mMeshes[0]];
		Mesh* crMesh = ProcessMesh(aiMesh, filename, entity->GetName(), pivot);
		entity->AddComponent(crMesh);
	}

	for(U32 i = 0; i < node->mNumChildren; ++i) {
		aiMatrix4x4 childChainTransform;
		aiVector3D childPivot(0.f, 0.f, 0.f);
		aiNode* childNode = CollapseFbxPivotsHelper(node->mChildren[i], childChainTransform, childPivot);
		Entity* child = CREATE Entity(childNode->mName.C_Str());
		child->SetParent(entity);
		ProcessNode(child, childNode, childChainTransform, childPivot, pivot, filename);
		entity->AddChild(child);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const String& filename, const String& groupID, const aiVector3D& pivot) {
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
		aiVector3D position = mesh->mVertices[i];
		if(bake_pivots_and_axes) {
			position -= pivot;
			position.x = -position.x;
			position.z = -position.z;
		}
		vertexBuffer->PushData((Byte*)&position, 3 * sizeof(float));

		if(vertexBuffer->HasTextureCoordinates()) {
			vertexBuffer->PushData((Byte*)&mesh->mTextureCoords[0][i], 2 * sizeof(float));
		}

		if(vertexBuffer->HasNormals()) {
			aiVector3D normal = mesh->mNormals[i];
			if(bake_pivots_and_axes) {
				normal.x = -normal.x;
				normal.z = -normal.z;
			}
			vertexBuffer->PushData((Byte*)&normal, 3 * sizeof(float));
		}

		if(vertexBuffer->HasTangents()) {
			aiVector3D tangent = mesh->mTangents[i];
			if(bake_pivots_and_axes) {
				tangent.x = -tangent.x;
				tangent.z = -tangent.z;
			}
			vertexBuffer->PushData((Byte*)&tangent, 3 * sizeof(float));
		}

		if(vertexBuffer->HasBitangents()) {
			aiVector3D bitangent = mesh->mBitangents[i];
			if(bake_pivots_and_axes) {
				bitangent.x = -bitangent.x;
				bitangent.z = -bitangent.z;
			}
			vertexBuffer->PushData((Byte*)&bitangent, 3 * sizeof(float));
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
