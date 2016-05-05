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
#include "Shaders/SimpleShader.h"
#include "Shaders/TextureShader.h"
#include "Shaders/LightCasterMaterialShader.h"
#include "Shaders/LightCasterDiffuseShader.h"
#include "Shaders/LightCasterDiffuseSpecularShader.h"
#include "Shaders/DirectionalLightShader.h"
#include "Shaders/PointLightShader.h"
#include "Shaders/SpotLightShader.h"
#include "Shaders/MultiLightShader.h"
#include "Utils/Light.h"
#include "Utils/LightCaster.h"
#include "Utils/DirectionalLight.h"
#include "Utils/PointLight.h"
#include "Utils/SpotLight.h"

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
	Model* model = new Model(*modelMeshes, texture);
	delete modelMeshes;
	string msg = "" + filename + " loaded in ";
	Debugger::Instance()->StopCheckTime(msg);
	launcher->LogIt("Poly Count: %d", model->GetPolyCount());
	return model;
}

Model* Graphics3D::LoadModel(const string& filename, Texture* diffuse, Texture* specular) {
	Debugger::Instance()->StartCheckTime();
	CRArray<Mesh*>* modelMeshes = LoadMeshes(filename);
	Model* model = new Model(*modelMeshes, diffuse, specular);
	delete modelMeshes;
	string msg = "" + filename + " loaded in ";
	Debugger::Instance()->StopCheckTime(msg);
	launcher->LogIt("Poly Count: %d", model->GetPolyCount());
	return model;
}

Mesh* Graphics3D::LoadMesh(const string& filename){
	Mesh* result = nullptr;
	CRArray<Mesh*>* meshes = LoadMeshes(filename);
	if(meshes->size() > 1){
		throw CrossException("File contains more than 1 mesh.\nUse LoadMeshes function.");
	}else{
		result = (*meshes)[0];
		delete meshes;
		return result;
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

void Graphics3D::DrawMeshSimple(Mesh* mesh, const Matrix& model, Color& color){
	SimpleShader* shader = (SimpleShader*)gfxGL->GetShader(Shader::Type::SIMPLE);
	PreDrawMesh(shader, mesh, model);

	SAFE(glUniform4fv(shader->uColor, 1, color.GetData()));

	PostDrawMesh(mesh);
}

void Graphics3D::DrawMeshTexture(Mesh* mesh, const Matrix& model, Texture* diffuse){
	TextureShader* shader = (TextureShader*)gfxGL->GetShader(Shader::Type::TEXTURE);
	PreDrawMesh(shader, mesh, model);

	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
	SAFE(glUniform1i(shader->uDiffuseTexture, 0));

	SAFE(glEnableVertexAttribArray(shader->aDiffuseCoords));
	SAFE(glVertexAttribPointer(shader->aDiffuseCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));

	PostDrawMesh(mesh);
}

void Graphics3D::DrawMeshLightCasterMaterial(Mesh* mesh, const Matrix& model, LightCaster* light, Material* material){
	LightCasterMaterialShader* shader = (LightCasterMaterialShader*)gfxGL->GetShader(Shader::Type::LIGHT_CASTER_MATERIAL);
	PreDrawMesh(shader, mesh, model);

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));

	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	SAFE(glUniform3fv(shader->uMaterialAmbient, 1, material->ambient.GetData()));
	SAFE(glUniform3fv(shader->uMaterialDiffuse, 1, material->diffuse.GetData()));
	SAFE(glUniform3fv(shader->uMaterialSpecular, 1, material->specular.GetData()));
	SAFE(glUniform1f(shader->uMaterialShininess, material->shininess * 128.f));

	SAFE(glUniform3fv(shader->uLightPosition, 1, light->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));

	SAFE(glEnableVertexAttribArray(shader->aNormal));
	SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 5));

	PostDrawMesh(mesh);
}

void Graphics3D::DrawMeshLightCasterDiffuse(Mesh* mesh, const Matrix& model, LightCaster* light, Texture* diffuse, Vector3D& specular, float shininess) {
	LightCasterDiffuseShader* shader = (LightCasterDiffuseShader*)gfxGL->GetShader(Shader::Type::LIGHT_CASTER_DIFFUSE);
	PreDrawMesh(shader, mesh, model);

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));
	
	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));

	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
	SAFE(glUniform1i(shader->uMaterialDiffuse, 0));

	SAFE(glUniform3fv(shader->uMaterialSpecular, 1, specular.GetData()));

	SAFE(glUniform3fv(shader->uLightPosition, 1, light->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));

	SAFE(glEnableVertexAttribArray(shader->aTexCoords));
	SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));
	SAFE(glEnableVertexAttribArray(shader->aNormal));
	SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 5));

	PostDrawMesh(mesh);
}

void Graphics3D::DrawMeshLightCasterDiffuseSpecular(Mesh* mesh, const Matrix& model, LightCaster* light, Texture* diffuse, Texture* specular, float shininess) {
	LightCasterDiffuseSpecularShader* shader = (LightCasterDiffuseSpecularShader*)gfxGL->GetShader(Shader::Type::LIGHT_CASTER_DIFFUSE_SPECULAR);
	PreDrawMesh(shader, mesh, model);

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));

	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));

	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
	SAFE(glUniform1i(shader->uMaterialDiffuse, 0));

	SAFE(glActiveTexture(GL_TEXTURE1));
	SAFE(glBindTexture(GL_TEXTURE_2D, specular->GetID()));
	SAFE(glUniform1i(shader->uMaterialSpecular, 1));

	SAFE(glUniform3fv(shader->uLightPosition, 1, light->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));

	SAFE(glEnableVertexAttribArray(shader->aTexCoords));
	SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));
	SAFE(glEnableVertexAttribArray(shader->aNormal));
	SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 5));

	PostDrawMesh(mesh);
}

void Graphics3D::DrawMeshDirectionalLight(Mesh* mesh, const Matrix& model, DirectionalLight* light, Texture* diffuse, Texture* specular, float shininess){
	DirectionalLightShader* shader = (DirectionalLightShader*)gfxGL->GetShader(Shader::Type::DIRECTIONAL_LIGHT);
	PreDrawMesh(shader, mesh, model);

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));

	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));

	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
	SAFE(glUniform1i(shader->uMaterialDiffuse, 0));

	SAFE(glActiveTexture(GL_TEXTURE1));
	SAFE(glBindTexture(GL_TEXTURE_2D, specular->GetID()));
	SAFE(glUniform1i(shader->uMaterialSpecular, 1));

	SAFE(glUniform3fv(shader->uLightDirection, 1, light->GetDirection().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));

	SAFE(glEnableVertexAttribArray(shader->aTexCoords));
	SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));
	SAFE(glEnableVertexAttribArray(shader->aNormal));
	SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 5));

	PostDrawMesh(mesh);
}

void Graphics3D::DrawMeshPointLight(Mesh* mesh, const Matrix& model, PointLight* light, Texture* diffuse, Texture* specular, float shininess){
	PointLightShader* shader = (PointLightShader*)gfxGL->GetShader(Shader::Type::POINT_LIGHT);
	PreDrawMesh(shader, mesh, model);

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));

	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));
	//material
	SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));

	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
	SAFE(glUniform1i(shader->uMaterialDiffuse, 0));

	SAFE(glActiveTexture(GL_TEXTURE1));
	SAFE(glBindTexture(GL_TEXTURE_2D, specular->GetID()));
	SAFE(glUniform1i(shader->uMaterialSpecular, 1));
	//light
	SAFE(glUniform3fv(shader->uLightPosition, 1, light->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));
	SAFE(glUniform1f(shader->uLightConstant, light->GetConstant()));
	SAFE(glUniform1f(shader->uLightLinear, light->GetLinear()));
	SAFE(glUniform1f(shader->uLightQuadratic, light->GetQuadratic()));

	SAFE(glEnableVertexAttribArray(shader->aTexCoords));
	SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));
	SAFE(glEnableVertexAttribArray(shader->aNormal));
	SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 5));

	PostDrawMesh(mesh);
}

void Graphics3D::DrawMeshSpotLight(Mesh* mesh, const Matrix& model, SpotLight* light, Texture* diffuse, Texture* specular, float shininess){
	SpotLightShader* shader = (SpotLightShader*)gfxGL->GetShader(Shader::Type::SPOT_LIGHT);
	PreDrawMesh(shader, mesh, model);

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));

	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));
	//material
	SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));

	SAFE(glActiveTexture(GL_TEXTURE0));
	SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
	SAFE(glUniform1i(shader->uMaterialDiffuse, 0));

	SAFE(glActiveTexture(GL_TEXTURE1));
	SAFE(glBindTexture(GL_TEXTURE_2D, specular->GetID()));
	SAFE(glUniform1i(shader->uMaterialSpecular, 1));
	//light
	SAFE(glUniform3fv(shader->uLightPosition, 1, light->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));
	SAFE(glUniform1f(shader->uLightConstant, light->GetConstant()));
	SAFE(glUniform1f(shader->uLightLinear, light->GetLinear()));
	SAFE(glUniform1f(shader->uLightQuadratic, light->GetQuadratic()));
	SAFE(glUniform1f(shader->uLightCutOff, cos(light->GetCutOff() / 180.f * PI)));
	SAFE(glUniform1f(shader->uLightOuterCutOff, cos((light->GetCutOff() + 10.f) / 180.f * PI)));
	SAFE(glUniform3fv(shader->uLightDirection, 1, light->GetDirection().GetData()));

	SAFE(glEnableVertexAttribArray(shader->aTexCoords));
	SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));
	SAFE(glEnableVertexAttribArray(shader->aNormal));
	SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 5));

	PostDrawMesh(mesh);
}

void Graphics3D::DrawMeshMultiLight(Mesh* mesh, const Matrix& model, const CRArray<PointLight*>& lights, Texture* diffuse, Texture* specular, float shininess){
	MultiLightShader* shader = (MultiLightShader*)gfxGL->GetShader(Shader::Type::MULTI_LIGHT);

	if( lights.size() <= shader->MaxPointLights ){
		gfxGL->UseShader(shader);
		SAFE(glEnable(GL_DEPTH_TEST));
		//vertex shader 
		Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
		mvp = mvp.Transpose();
		SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

		SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));
		
		Matrix normalMatrix = model.Inverse();
		SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
		//materials
		SAFE(glActiveTexture(GL_TEXTURE0));
		SAFE(glBindTexture(GL_TEXTURE_2D, diffuse->GetID()));
		SAFE(glUniform1i(shader->uMaterialDiffuse, 0));

		SAFE(glActiveTexture(GL_TEXTURE1));
		SAFE(glBindTexture(GL_TEXTURE_2D, specular->GetID()));
		SAFE(glUniform1i(shader->uMaterialSpecular, 1));

		SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));
		//lights
		SAFE(glUniform1i(shader->uPointLightCount, lights.size()));
		for(unsigned int i = 0; i < lights.size(); ++i){
			SAFE(glUniform3fv(shader->uPointLights[i].position, 1, lights[i]->GetPosition().GetData()));
			SAFE(glUniform3fv(shader->uPointLights[i].ambient, 1, lights[i]->GetAmbientStrength().GetData()));
			SAFE(glUniform3fv(shader->uPointLights[i].diffuse, 1, lights[i]->GetDiffuseStrength().GetData()));
			SAFE(glUniform3fv(shader->uPointLights[i].specular, 1, lights[i]->GetSpecularStrength().GetData()));
			SAFE(glUniform1f(shader->uPointLights[i].constant, lights[i]->GetConstant()));
			SAFE(glUniform1f(shader->uPointLights[i].linear, lights[i]->GetLinear()));
			SAFE(glUniform1f(shader->uPointLights[i].quadratic, lights[i]->GetQuadratic()));
		}

		SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

		SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBufferObject()));
		SAFE(glEnableVertexAttribArray(shader->aPosition));
		SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
		SAFE(glEnableVertexAttribArray(shader->aTexCoords));
		SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));
		SAFE(glEnableVertexAttribArray(shader->aNormal));
		SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 5));
		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetElementBufferObjet()));
		SAFE(glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0));
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

		SAFE(glDisable(GL_DEPTH_TEST));
	}else{
		throw CrossException("This shader can only draw %d point lights", shader->MaxPointLights);
	}
}

void Graphics3D::DrawModel(Model* model){
	switch(model->GetType()){
	case Model::Type::SOLID:
		for(Mesh* mesh : model->meshes){
			Color color = model->GetColor();
			DrawMeshSimple(mesh, model->GetModelMatrix(), color);
		}
		break;
	case Model::Type::TEXTURED:
		for(Mesh* mesh : model->meshes){
			DrawMeshTexture(mesh, model->GetModelMatrix(), model->GetDiffuseTexture());
		}
		break;
	case Model::Type::MATERIAL:
	default:
		throw CrossException("Unsupported model type for drawing");
	}
}

void Graphics3D::DrawModelLightCaster(Model* model, LightCaster* light){
	switch(model->GetType()){
	case Model::Type::MATERIAL:
		for(Mesh* mesh : model->meshes) {
			DrawMeshLightCasterMaterial(mesh, model->GetModelMatrix(), light, model->GetMaterial());
		}
		break;
	case Model::Type::TEXTURED:
		for(Mesh* mesh : model->meshes){
			if(model->HasSpecularMap()){
				DrawMeshLightCasterDiffuseSpecular(mesh, model->GetModelMatrix(), light, model->GetDiffuseTexture(), model->GetSpecularTexture(), 0.25f);
			}else{
				Vector3D specularStreanth(0.5f);
				DrawMeshLightCasterDiffuse(mesh, model->GetModelMatrix(), light, model->GetDiffuseTexture(), specularStreanth, 0.5f);
			}
		}
		break;
	case Model::Type::SOLID:
	default:
		throw CrossException("Unsupported model type for drawing");
		break;
	}
}

void Graphics3D::DrawModelDirectLight(Model* model, DirectionalLight* light){
	if(model->GetType() == Model::Type::TEXTURED){
		if(model->HasSpecularMap()){
			for(Mesh* mesh : model->meshes){
				DrawMeshDirectionalLight(mesh, model->GetModelMatrix(), light, model->GetDiffuseTexture(), model->GetSpecularTexture(), 0.4f);
			}
		}else{
			throw CrossException("Model needs to be with specular map");
		}
	}else{
		throw CrossException("Expected TEXTURED model type");
	}
}

void Graphics3D::DrawModelPointLight(Model* model, PointLight* light){
	if(model->GetType() == Model::Type::TEXTURED){
		if(model->HasSpecularMap()){
			for(Mesh* mesh : model->meshes){
				DrawMeshPointLight(mesh, model->GetModelMatrix(), light, model->GetDiffuseTexture(), model->GetSpecularTexture(), 0.4f);
			}
		}else{
			throw CrossException("Model needs to be with specular map");
		}
	}else{
		throw CrossException("Expected TEXTURED model type");
	}
}

void Graphics3D::DrawModelSpotLight(Model* model, SpotLight* light){
	if(model->GetType() == Model::Type::TEXTURED){
		if(model->HasSpecularMap()){
			for(Mesh* mesh : model->meshes){
				DrawMeshSpotLight(mesh, model->GetModelMatrix(), light, model->GetDiffuseTexture(), model->GetSpecularTexture(), 0.4f);
			}
		}else{
			throw CrossException("Model needs to be with specular map");
		}
	}else{
		throw CrossException("Expected TEXTURED model type");
	}
}

void Graphics3D::DrawModelMultiLight(Model* model, const CRArray<PointLight*>& lights){
	if(model->GetType() == Model::Type::TEXTURED){
		if(model->HasSpecularMap()){
			for(Mesh* mesh : model->meshes){
				DrawMeshMultiLight(mesh, model->GetModelMatrix(), lights, model->GetDiffuseTexture(), model->GetSpecularTexture(), 0.4f);
			}
		}else{
			throw CrossException("Model needs to be with specular map");
		}
	}else{
		throw CrossException("Expected TEXTURED model type");
	}
}

void Graphics3D::PreDrawMesh(Shader* shader, Mesh* mesh, const Matrix& model){
	gfxGL->UseShader(shader);

	SAFE(glEnable(GL_DEPTH_TEST));

	SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBufferObject()));

	SAFE(glEnableVertexAttribArray(shader->aPosition));
	SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));

	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
}

void Graphics3D::PostDrawMesh(Mesh* mesh){
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