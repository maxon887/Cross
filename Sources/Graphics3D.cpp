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
#include "Shaders/LightMaterialShader.h"
#include "Shaders/LightDiffuseShader.h"
#include "Shaders/LightDiffuseSpecularShader.h"
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

Model* Graphics3D::LoadModel(const string& filename){
	Debugger::Instance()->StartCheckTime();
	CRArray<Mesh*>* modelMeshes = LoadMeshes(filename);
	Model* model = new Model(*modelMeshes);
	delete modelMeshes;
	string msg = "" + filename + " loaded in ";
	Debugger::Instance()->StopCheckTime(msg);
	launcher->LogIt("Poly Count: %d", model->GetPolyCount());
	return model;
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
	Model* model = new Model(*modelMeshes);
	model->SetDiffuseTexture(texture);
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
	model->SetDiffuseTexture(diffuse);
	model->SetSpecularTexture(specular);
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
	string modelFilePath = launcher->PathFromFile(filename);
	ProcessNode(meshes, scene->mRootNode, scene, modelFilePath);
	if(meshes->size() == 0){
		throw CrossException("Cannot load meshes from file");
	}
	return meshes;
}

void Graphics3D::DrawMeshSimple(Mesh* mesh, const Matrix& model, Color& color){
	SimpleShader* shader = (SimpleShader*)gfxGL->GetShader(Shader::Type::SIMPLE);
	gfxGL->UseShader(shader);

	//transfer uniforms
	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glUniform4fv(shader->uColor, 1, color.GetData()));

	BindAttributes(shader, mesh);

	//drawing
	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshTexture(Mesh* mesh, const Matrix& model){
	TextureShader* shader = (TextureShader*)gfxGL->GetShader(Shader::Type::TEXTURE);
	gfxGL->UseShader(shader);
	//transfer uniforms
	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	BindTextures(shader, mesh);

	Color white = Color::White;
	SAFE(glUniform4fv(shader->uColor, 1, white.GetData()));
	
	BindAttributes(shader, mesh);

	//drawing
	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshLightMaterial(Mesh* mesh, const Matrix& model, LightCaster* light, Material* material){
	LightMaterialShader* shader = (LightMaterialShader*)gfxGL->GetShader(Shader::Type::LIGHT_MATERIAL);
	gfxGL->UseShader(shader);
	//transfer uniforms
	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));

	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));

	SAFE(glUniform3fv(shader->uMaterialAmbient, 1, material->ambient.GetData()));
	SAFE(glUniform3fv(shader->uMaterialDiffuse, 1, material->diffuse.GetData()));
	SAFE(glUniform3fv(shader->uMaterialSpecular, 1, material->specular.GetData()));
	SAFE(glUniform1f(shader->uMaterialShininess, material->shininess * 128.f));

	SAFE(glUniform3fv(shader->uLightPosition, 1, light->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));

	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	BindAttributes(shader, mesh);

	//drawing
	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshLightDiffuse(Mesh* mesh, const Matrix& model, LightCaster* light, Vector3D& specular, float shininess) {
	LightDiffuseShader* shader = (LightDiffuseShader*)gfxGL->GetShader(Shader::Type::LIGHT_DIFFUSE);
	gfxGL->UseShader(shader);
	//transfer uniforms
	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));
	
	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));

	BindTextures(shader, mesh);

	SAFE(glUniform3fv(shader->uMaterialSpecular, 1, specular.GetData()));

	SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));

	SAFE(glUniform3fv(shader->uLightPosition, 1, light->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));

	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	BindAttributes(shader, mesh);

	//drawing
	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshLightDiffuseSpecular(Mesh* mesh, const Matrix& model, LightCaster* light, float shininess) {
	LightDiffuseSpecularShader* shader = (LightDiffuseSpecularShader*)gfxGL->GetShader(Shader::Type::LIGHT_DIFFUSE_SPECULAR);
	gfxGL->UseShader(shader);
	//transfer uniforms
	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));

	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));

	BindTextures(shader, mesh);

	SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));

	SAFE(glUniform3fv(shader->uLightPosition, 1, light->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));

	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	BindAttributes(shader, mesh);

	//drawing
	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshDirectionalLight(Mesh* mesh, const Matrix& model, DirectionalLight* light, float shininess){
	DirectionalLightShader* shader = (DirectionalLightShader*)gfxGL->GetShader(Shader::Type::DIRECTIONAL_LIGHT);
	gfxGL->UseShader(shader);
	//transfer uniforms
	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));

	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));

	BindTextures(shader, mesh);

	SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));

	SAFE(glUniform3fv(shader->uLightDirection, 1, light->GetDirection().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));

	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	BindAttributes(shader, mesh);

	//drawing
	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshPointLight(Mesh* mesh, const Matrix& model, PointLight* light, float shininess){
	PointLightShader* shader = (PointLightShader*)gfxGL->GetShader(Shader::Type::POINT_LIGHT);
	gfxGL->UseShader(shader);
	//transfer uniforms
	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));

	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));

	BindTextures(shader, mesh);

	SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));

	SAFE(glUniform3fv(shader->uLightPosition, 1, light->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));
	SAFE(glUniform1f(shader->uLightConstant, light->GetConstant()));
	SAFE(glUniform1f(shader->uLightLinear, light->GetLinear()));
	SAFE(glUniform1f(shader->uLightQuadratic, light->GetQuadratic()));

	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	//transfer attributes
	BindAttributes(shader, mesh);

	//drawing
	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshSpotLight(Mesh* mesh, const Matrix& model, SpotLight* light, float shininess){
	SpotLightShader* shader = (SpotLightShader*)gfxGL->GetShader(Shader::Type::SPOT_LIGHT);
	gfxGL->UseShader(shader);

	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));

	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));
	//material
	SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));

	BindTextures(shader, mesh);
	//light
	SAFE(glUniform3fv(shader->uLightPosition, 1, light->GetPosition().GetData()));
	SAFE(glUniform3fv(shader->uLightAmbient, 1, light->GetAmbientStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightDiffuse, 1, light->GetDiffuseStrength().GetData()));
	SAFE(glUniform3fv(shader->uLightSpecular, 1, light->GetSpecularStrength().GetData()));
	SAFE(glUniform1f(shader->uLightConstant, light->GetConstant()));
	SAFE(glUniform1f(shader->uLightLinear, light->GetLinear()));
	SAFE(glUniform1f(shader->uLightQuadratic, light->GetQuadratic()));
	SAFE(glUniform1f(shader->uLightCutOff, cos(light->GetCutOff() / 180.f * PI)));
	SAFE(glUniform1f(shader->uLightOuterCutOff, cos(light->GetOuterCutOff() / 180.f * PI)));
	SAFE(glUniform3fv(shader->uLightDirection, 1, light->GetDirection().GetData()));

	BindAttributes(shader, mesh);

	//drawing
	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawMeshMultiLight(Mesh* mesh,
									const Matrix& model,
									const CRArray<PointLight*>& pointLights,
									const CRArray<DirectionalLight*>& directLights,
									const CRArray<SpotLight*>& spotLights,
									float shininess)
{
	MultiLightShader* shader = (MultiLightShader*)gfxGL->GetShader(Shader::Type::MULTI_LIGHT);

	if(pointLights.size() > shader->MaxPointLights){
		throw CrossException("This shader can only draw %d point lights", shader->MaxPointLights);
	}

	if(directLights.size() > shader->MaxDirectionalLights){
		throw CrossException("This shader can only draw %d directional lights", shader->MaxDirectionalLights);
	}

	if(spotLights.size() > shader->MaxSpotLights){
		throw CrossException("This shader can only draw %d spot lights", shader->MaxSpotLights);
	}

	gfxGL->UseShader(shader);
	//vertex shader 
	Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));

	SAFE(glUniformMatrix4fv(shader->uModelMatrix, 1, GL_FALSE, model.Transpose().GetData()));
		
	Matrix normalMatrix = model.Inverse();
	SAFE(glUniformMatrix4fv(shader->uNormalMatrix, 1, GL_FALSE, normalMatrix.GetData()));
	//materials
	BindTextures(shader, mesh);

	SAFE(glUniform1f(shader->uMaterialShininess, shininess * 128.f));
	//lights
	SAFE(glUniform1i(shader->uPointLightCount, pointLights.size()));
	for(unsigned int i = 0; i < pointLights.size(); ++i){
		SAFE(glUniform3fv(shader->uPointLights[i].position, 1, pointLights[i]->GetPosition().GetData()));
		SAFE(glUniform3fv(shader->uPointLights[i].ambient, 1, pointLights[i]->GetAmbientStrength().GetData()));
		SAFE(glUniform3fv(shader->uPointLights[i].diffuse, 1, pointLights[i]->GetDiffuseStrength().GetData()));
		SAFE(glUniform3fv(shader->uPointLights[i].specular, 1, pointLights[i]->GetSpecularStrength().GetData()));
		SAFE(glUniform1f(shader->uPointLights[i].constant, pointLights[i]->GetConstant()));
		SAFE(glUniform1f(shader->uPointLights[i].linear, pointLights[i]->GetLinear()));
		SAFE(glUniform1f(shader->uPointLights[i].quadratic, pointLights[i]->GetQuadratic()));
	}

	SAFE(glUniform1i(shader->uDirectionalLightCount, directLights.size()));
	for(unsigned int i = 0; i < directLights.size(); ++i){
		SAFE(glUniform3fv(shader->uDirectionalLights[i].direction, 1, directLights[i]->GetDirection().GetData()));
		SAFE(glUniform3fv(shader->uDirectionalLights[i].ambient, 1, directLights[i]->GetAmbientStrength().GetData()));
		SAFE(glUniform3fv(shader->uDirectionalLights[i].diffuse, 1, directLights[i]->GetDiffuseStrength().GetData()));
		SAFE(glUniform3fv(shader->uDirectionalLights[i].specular, 1, directLights[i]->GetSpecularStrength().GetData()));
	}

	SAFE(glUniform1i(shader->uSpotLightCount, spotLights.size()));
	for(unsigned int i = 0; i < spotLights.size(); ++i){
		SAFE(glUniform3fv(shader->uSpotLights[i].position, 1, spotLights[i]->GetPosition().GetData()));
		SAFE(glUniform3fv(shader->uSpotLights[i].ambient, 1, spotLights[i]->GetAmbientStrength().GetData()));
		SAFE(glUniform3fv(shader->uSpotLights[i].diffuse, 1, spotLights[i]->GetDiffuseStrength().GetData()));
		SAFE(glUniform3fv(shader->uSpotLights[i].specular, 1, spotLights[i]->GetSpecularStrength().GetData()));
		SAFE(glUniform1f(shader->uSpotLights[i].constant, spotLights[i]->GetConstant()));
		SAFE(glUniform1f(shader->uSpotLights[i].linear, spotLights[i]->GetLinear()));
		SAFE(glUniform1f(shader->uSpotLights[i].quadratic, spotLights[i]->GetQuadratic()));
		SAFE(glUniform1f(shader->uSpotLights[i].cut_off, cos(spotLights[i]->GetCutOff() / 180.f * PI)));
		SAFE(glUniform1f(shader->uSpotLights[i].outer_cut_off, cos(spotLights[i]->GetOuterCutOff() / 180.f * PI)));
		SAFE(glUniform3fv(shader->uSpotLights[i].direction, 1, spotLights[i]->GetDirection().GetData()));
	}

	SAFE(glUniform3fv(shader->uCameraPosition, 1, camera->GetPosition().GetData()));

	BindAttributes(shader, mesh);

	//drawing
	SAFE(glEnable(GL_DEPTH_TEST));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glDisable(GL_DEPTH_TEST));
}

void Graphics3D::DrawModelSimple(Model* model){
	for(Mesh* mesh : model->meshes){
		Color color = model->GetColor();
		DrawMeshSimple(mesh, model->GetModelMatrix() * mesh->GetModelMatrix(), color);
	}
}

void Graphics3D::DrawModelTexture(Model* model){
	for(Mesh* mesh : model->meshes){
		DrawMeshTexture(mesh, model->GetModelMatrix() * mesh->GetModelMatrix());
	}
}

void Graphics3D::DrawModelLightMaterial(Model* model, LightCaster* light){
	for(Mesh* mesh : model->meshes){
		DrawMeshLightMaterial(mesh, model->GetModelMatrix() * mesh->GetModelMatrix(), light, model->GetMaterial());
	}
}

void Graphics3D::DrawModelLightDiffuse(Model* model, LightCaster* light){
	Vector3D specularStength(0.5f);
	for(Mesh* mesh : model->meshes){
		DrawMeshLightDiffuse(mesh, model->GetModelMatrix() * mesh->GetModelMatrix(), light, specularStength, 0.5f);
	}
}

void Graphics3D::DrawModelLightDiffuseSpecular(Model* model, LightCaster* light){
	for(Mesh* mesh : model->meshes){
		DrawMeshLightDiffuseSpecular(mesh, model->GetModelMatrix() * mesh->GetModelMatrix(), light, 0.5f);
	}
}

void Graphics3D::DrawModelDirectLight(Model* model, DirectionalLight* light){
	for(Mesh* mesh : model->meshes){
		DrawMeshDirectionalLight(mesh, model->GetModelMatrix() * mesh->GetModelMatrix(), light, 0.5f);
	}
}

void Graphics3D::DrawModelPointLight(Model* model, PointLight* light){
	for(Mesh* mesh : model->meshes){
		DrawMeshPointLight(mesh, model->GetModelMatrix() * mesh->GetModelMatrix(), light, 0.5f);
	}
}

void Graphics3D::DrawModelSpotLight(Model* model, SpotLight* light){
	for(Mesh* mesh : model->meshes){
		DrawMeshSpotLight(mesh, model->GetModelMatrix() * mesh->GetModelMatrix(), light, 0.5f);
	}
}

void Graphics3D::DrawModelMultiLight(Model* model,	const CRArray<PointLight*>& pointLights, 
													const CRArray<DirectionalLight*>& directLights,
													const CRArray<SpotLight*>& spotLights){
	for(Mesh* mesh : model->meshes){
		DrawMeshMultiLight(mesh, model->GetModelMatrix() * mesh->GetModelMatrix(), pointLights, directLights, spotLights, 0.5f);
	}
}

void Graphics3D::BindTextures(Shader* shader, Mesh* mesh){
	for(unsigned int i = 0; i < Shader::MaxDiffuseMaps; ++i){
		if(shader->uDiffuseMaps[i] != -1){
			if(mesh->diffuse_maps.size() > i){
				SAFE(glActiveTexture(GL_TEXTURE0 + i));
				SAFE(glBindTexture(GL_TEXTURE_2D, mesh->diffuse_maps[i]->GetID()));
				SAFE(glUniform1i(shader->uDiffuseMaps[i], i));
			}else{
				throw CrossException("Mesh does not contain %d diffuse map", i);
			}
		}else{
			break;
		}
	}
	for(unsigned int i = 0; i < Shader::MaxSpecularMaps; ++i){
		if(shader->uSpecularMaps[i] != -1){
			if(mesh->specular_maps.size() > i){
				SAFE(glActiveTexture(GL_TEXTURE0 + Shader::MaxDiffuseMaps + i));
				SAFE(glBindTexture(GL_TEXTURE_2D, mesh->specular_maps[i]->GetID()));
				SAFE(glUniform1i(shader->uSpecularMaps[i], Shader::MaxDiffuseMaps + i));
			}else{
				throw CrossException("Mesh does not contain %d specular map", i);
			}
		}else{
			break;
		}
	}
}

void Graphics3D::BindAttributes(Shader* shader, Mesh* mesh){
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO));
	if(shader->aPosition != -1){
		SAFE(glEnableVertexAttribArray(shader->aPosition));
		SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 0));
	}
	if(shader->aTexCoords != -1){
		SAFE(glEnableVertexAttribArray(shader->aTexCoords));
		SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 3));
	}
	if(shader->aNormal != -1){
		SAFE(glEnableVertexAttribArray(shader->aNormal));
		SAFE(glVertexAttribPointer(shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLfloat*)0 + 5));
	}
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
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

Mesh* Graphics3D::ProcessMesh(aiMesh* mesh, const aiScene* scene, const string& modelFilePath){
	CRArray<Vertex> vertices;
	CRArray<unsigned int> indices;

	for(unsigned int i = 0; i < mesh->mNumVertices; ++i){
		Vertex vertex;

		if(mesh->mVertices){
			vertex.pos.x = mesh->mVertices[i].x;
			vertex.pos.y = mesh->mVertices[i].y;
			vertex.pos.z = mesh->mVertices[i].z;
		}else{
			throw CrossException("Model file does not vertex position data");
		}

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
	CRArray<Texture*>* diffuseMaps;
	CRArray<Texture*>* specularMaps;

	if(mesh->mMaterialIndex >= 0){
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString matName;
		material->Get(AI_MATKEY_NAME, matName);
		launcher->LogIt(matName.C_Str());
		diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE, modelFilePath);
		specularMaps = LoadTextures(material, aiTextureType_SPECULAR, modelFilePath);

	}

	Mesh* ret = new Mesh(vertices, indices, *diffuseMaps, *specularMaps,  mesh->mNumFaces);
	delete diffuseMaps;
	delete specularMaps;
	return ret;
}

void Graphics3D::ProcessNode(CRArray<Mesh*>* meshes, aiNode* node, const aiScene* scene, const string& modelFilePath){
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* crMesh = ProcessMesh(aiMesh, scene, modelFilePath);
		Matrix model = Matrix::CreateZero();
		memcpy(model.m, &node->mTransformation.a1, sizeof(float) * 16);
		crMesh->SetModelMatrix(model);
		meshes->push_back(crMesh);
	}
	for(unsigned int i = 0; i < node->mNumChildren; ++i){
		ProcessNode(meshes, node->mChildren[i], scene, modelFilePath);
	}
}

void Graphics3D::WindowResizeHandle(int width, int height){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera->SetProjectionMatrix(projection);
}