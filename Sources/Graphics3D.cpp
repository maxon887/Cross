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

#define SWIG

#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"

using namespace cross;

Mesh* processMesh(aiMesh* mesh, const aiScene* scene){
	CRArray<Vertex>* vertices = new CRArray<Vertex>();
	CRArray<unsigned int>* indices = new CRArray<unsigned int>();

	for(unsigned int i = 0; i < mesh->mNumVertices; ++i){
		Vertex vertex;
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;
		vertices->push_back(vertex);
	}

	for(unsigned int i = 0; i < mesh->mNumFaces; ++i){
		//aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; ++j){
			indices->push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
	Mesh* ret = new Mesh(*vertices, *indices);
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
	camera->SetPosition(Vector3D(0.f, 0.f, -1.f));
	triangle_shader = new TriangleShaders();
}

Camera* Graphics3D::GetCamera(){
	return camera;
}

Mesh* Graphics3D::LoadMesh(string filename){
	File* file = launcher->LoadFile(filename);
	Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
	const aiScene* scene = importer.ReadFileFromMemory(file->data, file->size, aiProcess_Triangulate | aiProcess_FlipUVs);
	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		throw CrossException("Assimp Error: %s", importer.GetErrorString());
	}
	CRArray<Mesh*> meshes;
	processNode(&meshes, scene->mRootNode, scene);
	if(meshes.size() > 1){
		//throw CrossException("Model file containes more than 1 mesh");
	}
	return meshes[1];
}

void Graphics3D::DrawMesh(Mesh* mesh, const Matrix& transform){
	gfxGL->UseProgram(triangle_shader->program);
	if(triangle_shader->aPosition != -1){
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO));
		SAFE(glEnableVertexAttribArray(triangle_shader->aPosition));
		SAFE(glVertexAttribPointer(triangle_shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
	}
	if(triangle_shader->uMVP != -1){
		Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * transform;
		mvp = mvp.Transpose();
		SAFE(glUniformMatrix4fv(triangle_shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	}
	//glDrawArrays(GL_TRIANGLES, 0,
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
}