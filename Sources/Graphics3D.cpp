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
#include "Launcher.h"
#include "Mesh.h"
#include "File.h"
#include "Model.h"
#include "System/Debugger.h"
#include "Scene.h"
#include "Light.h"

#include <algorithm>

#define SWIG

#include "Libs/Assimp/Importer.hpp"
#include "Libs/Assimp/scene.h"
#include "Libs/Assimp/postprocess.h"
#include "Libs/Assimp/version.h"

using namespace cross;

Graphics3D::Graphics3D():
	current_translation(Matrix::Identity),
	current_rotation(Matrix::Identity),
	current_scaling(Matrix::Identity),
	current_geotranslation(Matrix::Identity)
{
	launcher->LogIt("Graphics3D::Graphics3D()");
	U32 major = aiGetVersionMajor();
	U32 minor = aiGetVersionMinor();
	launcher->LogIt("\tUse assimp version %d.%d", major, minor);
}

Graphics3D::~Graphics3D(){
}

Model* Graphics3D::LoadPrimitive(Graphics3D::Primitives primitive){
	switch(primitive) {
	case cross::Graphics3D::CUBE:
		return LoadModel("Engine/Models/Cube.obj");
	case cross::Graphics3D::SPHERE:
		return LoadModel("Engine/Models/Sphere.obj");
	case cross::Graphics3D::PLANE:
		return LoadModel("Engine/Models/Plane.obj");
	default:
		throw CrossException("Unknown primitive type");
	}
}

Model* Graphics3D::LoadModel(const string& filename){
	Debugger::Instance()->SetTimeCheck();
	Model* model = new Model(filename);
	ProcessScene(model);
	float loadTime = Debugger::Instance()->GetTimeCheck();
	launcher->LogIt("Model(%s) loaded in %0.1fms", filename.c_str(), loadTime);
	return model;
}

void Graphics3D::ProcessScene(Model* model){
	File* file = launcher->LoadFile(model->GetName());
	Assimp::Importer importer;
	current_scene = importer.ReadFileFromMemory(file->data, file->size, aiProcess_Triangulate | aiProcess_FlipUVs);
	delete file;
	if(!current_scene || current_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !current_scene->mRootNode){
		throw CrossException("Assimp Error: %s", importer.GetErrorString());
	}
	ProcessNode(model, current_scene->mRootNode);
}

void Graphics3D::ProcessNode(Model* model, aiNode* node){
	for(U32 i = 0; i < node->mNumMeshes; i++){
		aiMesh* aiMesh = current_scene->mMeshes[node->mMeshes[i]];
		Mesh* crMesh = ProcessMesh(aiMesh);
		crMesh->SetName(node->mName.C_Str());
		if(model->GetFormat() == Model::Format::FBX){
			crMesh->SetModelMatrix(current_translation * current_rotation * current_scaling * current_geotranslation);
			current_translation = Matrix::Identity;
			current_rotation = Matrix::Identity;
			current_scaling = Matrix::Identity;
			current_geotranslation = Matrix::Identity;
		}else{
			Matrix modelMat = Matrix::Zero;
			memcpy(modelMat.m, &node->mTransformation.a1, sizeof(float) * 16);
			crMesh->SetModelMatrix(modelMat);
		}
		model->AddMesh(crMesh);
	}
	if(model->GetFormat() == Model::Format::FBX){
		string nodeName = node->mName.C_Str();
		if(nodeName.find("Translation") != std::string::npos){
			if(nodeName.find("Geometric") != std::string::npos){
				memcpy(current_geotranslation.m, &node->mTransformation.a1, sizeof(float) * 16);
			}else{
				memcpy(current_translation.m, &node->mTransformation.a1, sizeof(float) * 16);
			}
		}
		if(nodeName.find("Scaling") != std::string::npos){
			memcpy(current_scaling.m, &node->mTransformation.a1, sizeof(float) * 16);
		}
		if(nodeName.find("Rotation") != std::string::npos){
			if(current_translation == Matrix::Identity){
			memcpy(current_translation.m, &node->mTransformation.a1, sizeof(float) * 16);
			}
		}
	}
	for(U32 i = 0; i < node->mNumChildren; ++i){
		ProcessNode(model, node->mChildren[i]);
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

	for(U32 i = 0; i < mesh->mNumVertices; ++i){
		vertexBuffer->PushData((unsigned char*)&mesh->mVertices[i], 3 * sizeof(float));

		if(vertexBuffer->HasTextureCoordinates()){
			vertexBuffer->PushData((unsigned char*)&mesh->mTextureCoords[0][i], 2 * sizeof(float));
		}

		if(vertexBuffer->HasNormals()){
			vertexBuffer->PushData((unsigned char*)&mesh->mNormals[i], 3 * sizeof(float));
		}
	}

	CRArray<U32> indices;
	for(U32 i = 0; i < mesh->mNumFaces; ++i){
		for(U32 j = 0; j < mesh->mFaces[i].mNumIndices; ++j){
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	return new Mesh(vertexBuffer, indices, mesh->mNumFaces);
}