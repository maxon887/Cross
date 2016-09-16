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
#include "Utils/Debugger.h"
#include "Graphics2D.h"
#include "Texture.h"
#include "Material.h"
#include "Scene.h"
#include "Utils/Light.h"

#include <algorithm>

#define SWIG

#include "Libs/Assimp/Importer.hpp"
#include "Libs/Assimp/scene.h"
#include "Libs/Assimp/postprocess.h"
#include "Libs/Assimp/version.h"

using namespace cross;

Graphics3D::Graphics3D():
	current_translation(Matrix::CreateIdentity()),
	current_rotation(Matrix::CreateIdentity()),
	current_scaling(Matrix::CreateIdentity()),
	current_geotranslation(Matrix::CreateIdentity())
{
	launcher->LogIt("Graphics3D::Graphics3D()");
	unsigned int major = aiGetVersionMajor();
	unsigned int minor = aiGetVersionMinor();
	launcher->LogIt("Use assimp version %d.%d", major, minor);
}

Graphics3D::~Graphics3D(){
}

Model* Graphics3D::LoadModel(const string& filename){
	Debugger::Instance()->StartCheckTime();
	Model* model = new Model(filename);
	ProcessScene(model);
	string msg = "" + filename + " loaded in ";
	Debugger::Instance()->StopCheckTime(msg);
	launcher->LogIt("Poly Count: %d", model->GetPolyCount());
	return model;
}

Scene* Graphics3D::LoadScene(const string& filename){
	Scene* newScene = new Scene();
	//lights
	//newScene->SetAmbientColor(Color::White);
	Light* light = new Light(Light::Type::POINT);
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	newScene->AddLight(light);
	return newScene;
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
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		aiMesh* aiMesh = current_scene->mMeshes[node->mMeshes[i]];
		Mesh* crMesh = ProcessMesh(aiMesh);
		if(model->GetFormat() == Model::Format::FBX){
			crMesh->SetModelMatrix(current_translation * current_rotation * current_scaling * current_geotranslation);
			current_translation = Matrix::CreateIdentity();
			current_rotation = Matrix::CreateIdentity();
			current_scaling = Matrix::CreateIdentity();
			current_geotranslation = Matrix::CreateIdentity();
		}else{
			Matrix modelMat = Matrix::CreateZero();
			memcpy(modelMat.m, &node->mTransformation.a1, sizeof(float) * 16);
			crMesh->SetModelMatrix(modelMat);
		}
		model->AddMesh(crMesh);
	}
	if(model->GetFormat() == Model::Format::FBX){
		string nodeName = node->mName.C_Str();
		if(nodeName.find("Translation") != std::string::npos){
			if(nodeName.find("Geometric") != std::string::npos){
				Matrix translation = Matrix::CreateIdentity();
				memcpy(translation.m, &node->mTransformation.a1, sizeof(float) * 16);
				current_geotranslation = translation;
			}else{
				Matrix translation = Matrix::CreateIdentity();
				memcpy(translation.m, &node->mTransformation.a1, sizeof(float) * 16);
				current_translation = translation;
			}
		}
		if(nodeName.find("Scaling") != std::string::npos){
			Matrix scale = Matrix::CreateIdentity();
			memcpy(scale.m, &node->mTransformation.a1, sizeof(float) * 16);
			current_scaling = scale;
		}
		if(nodeName.find("Rotation") != std::string::npos){
			Matrix rotation = Matrix::CreateIdentity();
			memcpy(rotation.m, &node->mTransformation.a1, sizeof(float) * 16);
			current_rotation = rotation;
		}
	}
	for(unsigned int i = 0; i < node->mNumChildren; ++i){
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

	return new Mesh(vertexBuffer, indices, mesh->mNumFaces);
}