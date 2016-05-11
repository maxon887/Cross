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
#pragma once
#include "Cross.h"
#include "GraphicsGL.h"
#include "Events/Event.h"
#include "Shaders/Shader.h"

enum aiTextureType;
struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

namespace cross{

class Light;
class LightCaster;
class DirectionalLight;
class PointLight;
class SpotLight;

class Graphics3D{
public:
	Graphics3D();
	~Graphics3D();

	Camera* GetCamera();

	Mesh* LoadMesh(const string& filename);
	CRArray<Mesh*>* LoadMeshes(const string& filename);

	Model* LoadModel(const string& filename, const Color& color);
	Model* LoadModel(const string& filename, const Material& material);
	Model* LoadModel(const string& filename, Texture* diffuse);
	Model* LoadModel(const string& filename, Texture* diffuse, Texture* specular);

	//use "simple" shader
	void DrawMeshSimple(Mesh* mesh, const Matrix& model, Color& color);
	//use "texture" shader
	void DrawMeshTexture(Mesh* mesh, const Matrix& model, Texture* diffuse);
	//use "light_material" shader
	void DrawMeshLightMaterial(Mesh* mesh, const Matrix& model, LightCaster* light, Material* material);
	//use "light_diffuse" shader
	void DrawMeshLightDiffuse(Mesh* mesh, const Matrix& model, LightCaster* light, Texture* diffuse, Vector3D& specular, float shininess);
	//use "light_diffuse_specular" shader
	void DrawMeshLightDiffuseSpecular(Mesh* mesh, const Matrix& model, LightCaster* light, Texture* diffuse, Texture* specular, float shininess);
	//use "directional_light" shader
	void DrawMeshDirectionalLight(Mesh* mesh, const Matrix& model, DirectionalLight* light, Texture* diffuse, Texture* specular, float shininess);
	//use "point_light" shader
	void DrawMeshPointLight(Mesh* mesh, const Matrix& model, PointLight* light, Texture* diffuse, Texture* specualr, float shininess);
	void DrawMeshSpotLight(Mesh* mesh, const Matrix& model, SpotLight* light, Texture* diffuse, Texture* specular, float shininess);
	void DrawMeshMultiLight(	Mesh* mesh, 
								const Matrix& model, 
								const CRArray<PointLight*>& pointLights, 
								const CRArray<DirectionalLight*>& directLights, 
								const CRArray<SpotLight*>& spotLights,
								Texture* diffuse, 
								Texture* specular, 
								float shininess	);

	void DrawModel(Model* model);
	void DrawModelLightCaster(Model* model, LightCaster* light);
	void DrawModelDirectLight(Model* model, DirectionalLight* ligth);
	void DrawModelPointLight(Model* model, PointLight* light);
	void DrawModelSpotLight(Model* model, SpotLight* light);
	void DrawModelMultiLight(	Model* model,
								const CRArray<PointLight*>& pointLights, 
								const CRArray<DirectionalLight*>& directLights,
								const CRArray<SpotLight*>& spotLights);

private:
	Camera* camera;

	CRArray<Texture*>* LoadTextures(aiMaterial* material, aiTextureType type);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessNode(CRArray<Mesh*>* meshes, aiNode* node, const aiScene* scene);

	FastDelegate2<int, int, void> window_resize_handle;
	void WindowResizeHandle(int width, int height);
};

}
