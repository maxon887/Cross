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
#include "Game.h"
#include "Shaders/SimpleShader.h"
#include "Shaders/TextureShader.h"
#include "Shaders/LightMaterialShader.h"
#include "Shaders/SimplePointLightShader.h"
#include "Shaders/DirectionalLightShader.h"
#include "Utils/SimplePointLight.h"

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

		if(mesh->mNormals){
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}else{
			vertex.normal = Vector3D(0.f, 0.f, 0.f);
		}

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

Graphics3D::Graphics3D() : 
	camera(nullptr)
{
	launcher->LogIt("Graphics3D::Graphics3D()");
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera = new Camera(projection);

	window_resize_handle = MakeDelegate(this, &Graphics3D::WindowResizeHandle);
	game->WindowResized += window_resize_handle;
}

Graphics3D::~Graphics3D(){
	delete camera;
	game->WindowResized -= window_resize_handle;
}

Camera* Graphics3D::GetCamera(){
	return camera;
}

void Graphics3D::AddLightSource(SimplePointLight* light){
	light_sources.push_back(light);
}

void Graphics3D::ClearLightSources(){
	light_sources.clear();
}

Model* Graphics3D::LoadModel(Shader::Type shaderType, const string& modelFile){
	Debugger::Instance()->StartCheckTime();
	CRArray<Mesh*>* modelMeshes = LoadMeshes(modelFile);
	Model* model = new Model(shaderType);
	model->SetMeshes(*modelMeshes);
	delete modelMeshes;
	string msg = "" + modelFile + " loaded in ";
	Debugger::Instance()->StopCheckTime(msg);
	launcher->LogIt("Poly Count: %d", model->GetPolyCount());
	return model;
}

Mesh* Graphics3D::LoadMesh(const string& filename){
	CRArray<Mesh*>* meshes = LoadMeshes(filename);
	if(meshes->size() > 1){
		throw CrossException("File contains more than 1 mesh.\nUse LoadMeshes function.");
	}else{
		return (*meshes)[0];
	}
}

CRArray<Mesh*>* Graphics3D::LoadMeshes(const string& filename){
	File* file = launcher->LoadFile(filename);
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(file->data, file->size, aiProcess_Triangulate | aiProcess_FlipUVs);
	delete file;
	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		throw CrossException("Assimp Error: %s", importer.GetErrorString());
	}
	CRArray<Mesh*>* meshes = new CRArray<Mesh*>();
	processNode(meshes, scene->mRootNode, scene);
	if(meshes->size() == 0){
		throw CrossException("Cannot load meshes from file");
	}
	return meshes;
}

void Graphics3D::DrawMeshSimple(Mesh* mesh, const Matrix& transform, Color& color){
	SimpleShader* shader = (SimpleShader*)gfxGL->GetShader(Shader::Type::SIMPLE);
	gfxGL->UseShader(shader);

	SAFE(glEnable(GL_DEPTH_TEST));

	SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBufferObject()));

	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));

	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * transform;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glUniform4fv(shader->uColor, 1, color.GetData()));

	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetElementBufferObjet()));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshTexture(Mesh* mesh, const Matrix& transform, Texture* diffuse){
	TextureShader* shader = (TextureShader*)gfxGL->GetShader(Shader::Type::TEXTURE);
	gfxGL->UseShader(shader);

	SAFE(glEnable(GL_DEPTH_TEST));

	SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBufferObject()));

	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));

	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * transform;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glActiveTexture(GL_TEXTURE));
	SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
	SAFE(glUniform1i(shader->uDiffuseTexture, 0));

	SAFE(glEnableVertexAttribArray(shader->aDiffuseCoords));
	SAFE(glVertexAttribPointer(shader->aDiffuseCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));

	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetElementBufferObjet()));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshLightMaterial(Mesh* mesh, const Matrix& transform){
	LightMaterialShader* shader = (LightMaterialShader*)gfxGL->GetShader(Shader::Type::LIGHT_MATERIAL);
	gfxGL->UseShader(shader);

	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBufferObject()));

	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * transform;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	Matrix normalMatrix = transform.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	SAFE(glUniform3fv(shader->uMaterialAmbient, 1, mesh->GetMaterial().ambient.GetData()));
	SAFE(glUniform3fv(shader->uMaterialDiffuse, 1, mesh->GetMaterial().diffuse.GetData()));
	SAFE(glUniform3fv(shader->uMaterialSpecular, 1, mesh->GetMaterial().specular.GetData()));
	SAFE(glUniform1f(shader->uMaterialShininess, mesh->GetMaterial().shininess * 128.f));

	SAFE(glUniform3fv(shader->uLightPosition, 1, light_sources[0]->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light_sources[0]->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light_sources[0]->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light_sources[0]->GetSpecularStrength().GetData()));

	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
	SAFE(glEnableVertexAttribArray(shader->aNormal));
	SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 5));

	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetElementBufferObjet()));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshLightMaps(Mesh* mesh, const Matrix &transform, Texture* diffuse, Texture* specular){
	SimplePointLightShader* shader = (SimplePointLightShader*)gfxGL->GetShader(Shader::Type::SIMPLE_POINT_LIGHT);
	gfxGL->UseShader(shader);

	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBufferObject()));

	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * transform;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	Matrix normalMatrix = transform.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	SAFE(glUniform1f(shader->uMaterialShininess, mesh->GetMaterial().shininess * 128.f));

	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
	SAFE(glUniform1i(shader->uMaterialDiffuse, 0));

	SAFE(glActiveTexture(GL_TEXTURE1));
	SAFE(glBindTexture(GL_TEXTURE_2D, specular->GetID()));
	SAFE(glUniform1i(shader->uMaterialSpecular, 1));

	SAFE(glUniform3fv(shader->uLightPosition, 1, light_sources[0]->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light_sources[0]->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light_sources[0]->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light_sources[0]->GetSpecularStrength().GetData()));

	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
	SAFE(glEnableVertexAttribArray(shader->aTexCoords));
	SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));
	SAFE(glEnableVertexAttribArray(shader->aNormal));
	SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 5));

	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetElementBufferObjet()));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshLightCasters(Mesh* mesh, const Matrix& model, Vector3D& direction, Texture* diffuse, Texture* specular){
	DirectionalLightShader* shader = (DirectionalLightShader*)gfxGL->GetShader(Shader::Type::DIRECTIONAL_LIGHT);
	gfxGL->UseShader(shader);

	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBufferObject()));

	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	SAFE(glUniform1f(shader->uMaterialShininess, mesh->GetMaterial().shininess * 128.f));

	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
	SAFE(glUniform1i(shader->uMaterialDiffuse, 0));

	SAFE(glActiveTexture(GL_TEXTURE1));
	SAFE(glBindTexture(GL_TEXTURE_2D, specular->GetID()));
	SAFE(glUniform1i(shader->uMaterialSpecular, 1));

	SAFE(glUniform3fv(shader->uLightDirection, 1, direction.GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, Vector3D(0.2f).GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, Vector3D(1.f).GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, Vector3D(0.5f).GetData()));

	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
	SAFE(glEnableVertexAttribArray(shader->aTexCoords));
	SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));
	SAFE(glEnableVertexAttribArray(shader->aNormal));
	SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 5));

	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetElementBufferObjet()));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::WindowResizeHandle(int width, int height){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera->SetProjectionMatrix(projection);
}