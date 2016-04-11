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
#include "Graphics3D.h"
#include "Mesh.h"
#include "File.h"
#include "Camera.h"
#include "Model.h"
#include "Utils/Debugger.h"
#include "Graphics2D.h"
#include "Texture.h"

#define SWIG

#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"

using namespace cross;

Mesh* processMesh(aiMesh* mesh, const aiScene* scene){
	CRArray<Vertex> vertices;
	CRArray<unsigned int> indices;

	for(unsigned int i = 0; i < mesh->mNumVertices; ++i){
		Vertex vertex;
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		if(mesh->mTextureCoords[0]){
			vertex.uv.x = mesh->mTextureCoords[0][i].x;
			vertex.uv.y = mesh->mTextureCoords[0][i].y;
		}else{
			vertex.uv = Vector2D(0.f, 0.f);
		}

		vertices.push_back(vertex);
	}
	for(unsigned int i = 0; i < mesh->mNumFaces; ++i){
		for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; ++j){
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
	Mesh* ret = new Mesh(vertices, indices, mesh->mNumFaces);
	return ret;
}

void processNode(CRArray<Mesh*>* meshes, aiNode* node, const aiScene* scene){
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes->push_back(processMesh(mesh, scene));
	}
	for(unsigned int i = 0; i < node->mNumChildren; ++i){
		processNode(meshes, node->mChildren[i], scene);
	}
}



Graphics3D::Graphics3D(){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera = new Camera(projection);
	simple_shader = new SimpleShaders();
}

Camera* Graphics3D::GetCamera(){
	return camera;
}

CRArray<Mesh*> Graphics3D::LoadMeshes(const string& filename){
	File* file = launcher->LoadFile(filename);
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(file->data, file->size, aiProcess_Triangulate | aiProcess_FlipUVs);
	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		throw CrossException("Assimp Error: %s", importer.GetErrorString());
	}
	CRArray<Mesh*> meshes;
	processNode(&meshes, scene->mRootNode, scene);
	return meshes;
}

Model* Graphics3D::LoadModel(const string& model){
	return LoadModel(model, "");
}

Model* Graphics3D::LoadModel(const string& modelFile, const string& diffuseTexture){
	Debugger::Instance()->StartCheckTime();
	CRArray<Mesh*> modelMeshes = LoadMeshes(modelFile);
	Model* model = new Model(modelMeshes);
	if(!diffuseTexture.empty()){
		Texture* diffuse = gfx2D->LoadTexture(diffuseTexture);
		model->SetDiffuseTexture(diffuse);
	}
	string msg = "" + modelFile + " loaded in ";
	Debugger::Instance()->StopCheckTime(msg);
	launcher->LogIt("Poly Count: %d", model->GetPolyCount());
	return model;
}

void Graphics3D::DrawMesh(Mesh* mesh, const Matrix& transform, Texture* diffuse){
	gfxGL->UseShaders(simple_shader);

	SAFE(glEnable(GL_DEPTH_TEST));

	SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO));
	if(simple_shader->aPosition != -1){
		SAFE(glEnableVertexAttribArray(simple_shader->aPosition));
		SAFE(glVertexAttribPointer(simple_shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
	}
	if(simple_shader->uMVP != -1){
		Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * transform;
		mvp = mvp.Transpose();
		SAFE(glUniformMatrix4fv(simple_shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	}

	if(diffuse != nullptr && simple_shader->uDiffuseTexture != -1){
		SAFE(glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0));
		SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
		SAFE(glUniform1i(simple_shader->uDiffuseTexture, 0));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	}

	if(simple_shader->aDiffuseCoords != -1){
		SAFE(glEnableVertexAttribArray(simple_shader->aDiffuseCoords));
		SAFE(glVertexAttribPointer(simple_shader->aDiffuseCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));
	}

	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

	SAFE(glDisable(GL_DEPTH_TEST));

}