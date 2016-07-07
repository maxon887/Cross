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
#include "Mesh.h"
#include "File.h"
#include "Camera.h"
#include "Model.h"
#include "Utils/Debugger.h"
#include "Graphics2D.h"
#include "Texture.h"
#include "Game.h"
#include "Material.h"
#include "Shaders/LightDiffuseShader.h"
#include "Shaders/LightDiffuseSpecularShader.h"
#include "Shaders/DirectionalLightShader.h"
#include "Shaders/PointLightShader.h"
#include "Shaders/SpotLightShader.h"
#include "Shaders/MultiLightShader.h"
#include "Utils/Light.h"
#include "Utils/DirectionalLight.h"
#include "Utils/PointLight.h"
#include "Utils/SpotLight.h"

#define SWIG

#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"

using namespace cross;

Graphics3D::Graphics3D()
{
	launcher->LogIt("Graphics3D::Graphics3D()");
}

Graphics3D::~Graphics3D(){
}

Mesh* Graphics3D::LoadMesh(const string& filename){
	File* file = launcher->LoadFile(filename);
	Assimp::Importer importer;
	current_scene = importer.ReadFileFromMemory(file->data, file->size, aiProcess_Triangulate|aiProcess_FlipUVs);
	delete file;
	if(!current_scene || current_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !current_scene->mRootNode){
		throw CrossException("Assimp Error: %s", importer.GetErrorString());
	}
	return ProcessNode(current_scene->mRootNode);
}

Model* Graphics3D::LoadModel(const string& filename){
	Debugger::Instance()->StartCheckTime();
	CRArray<Mesh*>* modelMeshes = LoadMeshes(filename);
	Model* model = new Model();
	delete modelMeshes;
	string msg = "" + filename + " loaded in ";
	Debugger::Instance()->StopCheckTime(msg);
	launcher->LogIt("Poly Count: %d", model->GetPolyCount());
	return model;
}

Mesh* Graphics3D::ProcessNode(aiNode* node){
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		aiMesh* aiMesh = current_scene->mMeshes[node->mMeshes[i]];
		Mesh* crMesh = ProcessMesh(aiMesh);
		Matrix model = Matrix::CreateZero();
		memcpy(model.m, &node->mTransformation.a1, sizeof(float) * 16);
		crMesh->SetModelMatrix(model);
		return crMesh;
	}
	for(unsigned int i = 0; i < node->mNumChildren; ++i){
		return ProcessNode(node->mChildren[i]);
	}
	throw CrossException("Empty node");
}

Mesh* Graphics3D::ProcessMesh(aiMesh* mesh){
	VertexBuffer* vertexBuffer = new VertexBuffer();
	if(mesh->mTextureCoords[0]){
		vertexBuffer->UVEnabled(true);
	}
	if(mesh->mNormals){
		vertexBuffer->NarmalsEnabled(true);
	}

	for(unsigned int i = 0; i < mesh->mNumVertices; ++i){
		vertexBuffer->PushData((unsigned char*)&mesh->mVertices[i], 3 * sizeof(float));

		if(vertexBuffer->HasTextureCoordinates()){
			vertexBuffer->PushData((unsigned char*)&mesh->mTextureCoords[0][i], 2 * sizeof(float));
		}

		if(vertexBuffer->HasNormals()){
			vertexBuffer->PushData((unsigned char*)&mesh->mNormals[i], 3 * sizeof(float));
		}
	}

	CRArray<unsigned int> indices;
	for(unsigned int i = 0; i < mesh->mNumFaces; ++i){
		for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; ++j){
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	Mesh* crossMesh = new Mesh(vertexBuffer, indices, mesh->mNumFaces);/*
	if(mesh->mMaterialIndex >= 0){
		Material* crossMaterial = new Material(current_shader);
		aiMaterial* material = current_scene->mMaterials[mesh->mMaterialIndex];
		aiString matName;
		material->Get(AI_MATKEY_NAME, matName);
		launcher->LogIt(matName.C_Str());
		Texture* diffuseTexture = LoadTexture(material, aiTextureType_DIFFUSE, modelFilePath);
		crossMaterial->SetDiffuseTexture(diffuseTexture);
		crossMesh->SetMaterial(crossMaterial);
	}*/
	return crossMesh;
}

CRArray<Material*>* Graphics3D::LoadMaterials(aiScene* scene){
	CRArray<Material*>* materials = new CRArray<Material*>();
	for(int i = 0; i < scene->mNumMaterials; ++i){
		aiMaterial* aiMat = scene->mMaterials[i];
		aiString aiMatName;
		aiMat->Get(AI_MATKEY_NAME, aiMatName);
		Material* crMat = new Material(aiMatName.C_Str());
	}
	return nullptr;
}

/*
Model* Graphics3D::LoadModel(const string& filename, const Color& color){
	Debugger::Instance()->StartCheckTime();
	CRArray<Mesh*>* modelMeshes = LoadMeshes(filename);
	Model* model = new Model(*modelMeshes, color);
	delete modelMeshes;
	string msg = "" + filename + " loaded in ";
	Debugger::Instance()->StopCheckTime(msg);
	launcher->LogIt("Poly Count: %d", model->GetPolyCount());
	return model;
}

Model* Graphics3D::LoadModel(const string& filename, const Material& material){
	Debugger::Instance()->StartCheckTime();
	CRArray<Mesh*>* modelMeshes = LoadMeshes(filename);
	Model* model = new Model(*modelMeshes, material);
	delete modelMeshes;
	string msg = "" + filename + " loaded in ";
	Debugger::Instance()->StopCheckTime(msg);
	launcher->LogIt("Poly Count: %d", model->GetPolyCount());
	return model;
}

Model* Graphics3D::LoadModel(const string& filename, Texture* texture){
	Debugger::Instance()->StartCheckTime();
	CRArray<Mesh*>* modelMeshes = LoadMeshes(filename);
	Model* model = new Model(*modelMeshes);
	delete modelMeshes;
	string msg = "" + filename + " loaded in ";
	Debugger::Instance()->StopCheckTime(msg);
	launcher->LogIt("Poly Count: %d", model->GetPolyCount());
	return model;
}

Model* Graphics3D::LoadModel(const string& filename, Texture* diffuse, Texture* specular) {
	Debugger::Instance()->StartCheckTime();
	CRArray<Mesh*>* modelMeshes = LoadMeshes(filename);
	Model* model = new Model(*modelMeshes);
	delete modelMeshes;
	string msg = "" + filename + " loaded in ";
	Debugger::Instance()->StopCheckTime(msg);
	launcher->LogIt("Poly Count: %d", model->GetPolyCount());
	return model;
}*/

CRArray<Mesh*>* Graphics3D::LoadMeshes(const string& filename){
	File* file = launcher->LoadFile(filename);
	Assimp::Importer importer;
	current_scene = importer.ReadFileFromMemory(file->data, file->size, aiProcess_Triangulate | aiProcess_FlipUVs);
	delete file;
	if(!current_scene || current_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !current_scene->mRootNode){
		throw CrossException("Assimp Error: %s", importer.GetErrorString());
	}
	CRArray<Mesh*>* meshes = new CRArray<Mesh*>();
	string modelFilePath = launcher->PathFromFile(filename);
	//ProcessNode(meshes, current_scene->mRootNode, modelFilePath);
	if(meshes->size() == 0){
		throw CrossException("Cannot load meshes from file");
	}
	return meshes;
}

CRArray<Texture*>* Graphics3D::LoadTextures(aiMaterial* material, unsigned int type, const string& modelFilePath){
	CRArray<Texture*>* textures = new CRArray<Texture*>();
	for(unsigned int i = 0; i < material->GetTextureCount((aiTextureType)type); ++i){
		aiString str;
		material->GetTexture((aiTextureType)type, i, &str);

		string textureName = modelFilePath + "/"+ string(str.C_Str());
		Texture* texture = gfx2D->LoadTexture(textureName);
		textures->push_back(texture);
	}
	return textures;
}

Texture* Graphics3D::LoadTexture(aiMaterial* material, unsigned int type, const string& modelFilePath){
	if(material->GetTextureCount((aiTextureType)type) > 1){
		throw CrossException("Multiple textures not supported");
	}
	if(material->GetTextureCount((aiTextureType)type) == 0){
		return nullptr;
	}
	aiString textureName;
	material->GetTexture((aiTextureType)type, 0, &textureName);
	string filename = modelFilePath + "/"+ string(textureName.C_Str());
	Texture* texture = gfx2D->LoadTexture(filename);
	return texture;
}

void Graphics3D::ProcessNode(CRArray<Mesh*>* meshes, aiNode* node, const string& modelFilePath){
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		aiMesh* aiMesh = current_scene->mMeshes[node->mMeshes[i]];
		//Mesh* crMesh = ProcessMesh(aiMesh, modelFilePath);
		Matrix model = Matrix::CreateZero();
		memcpy(model.m, &node->mTransformation.a1, sizeof(float) * 16);
		//crMesh->SetModelMatrix(model);
		//meshes->push_back(crMesh);
	}
	for(unsigned int i = 0; i < node->mNumChildren; ++i){
		ProcessNode(meshes, node->mChildren[i], modelFilePath);
	}
}