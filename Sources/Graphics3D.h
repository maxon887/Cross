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

	void DrawMeshSimple(Mesh* mesh, const Matrix& model, Color& color);
	void DrawMeshTexture(Mesh* mesh, const Matrix& model, Texture* diffuse);
	void DrawMeshLightCasterMaterial(Mesh* mesh, const Matrix& model, LightCaster* light, Material* material);
	void DrawMeshLightCasterDiffuse(Mesh* mesh, const Matrix& model, LightCaster* light, Texture* diffuse, Vector3D& specular, float shininess);
	void DrawMeshLightCasterDiffuseSpecular(Mesh* mesh, const Matrix& model, LightCaster* light, Texture* diffuse, Texture* specular, float shininess);
	void DrawMeshDirectionalLight(Mesh* mesh, const Matrix& model, DirectionalLight* light, Texture* diffuse, Texture* specular, float shininess);
	void DrawMeshPointLight(Mesh* mesh, const Matrix& model, PointLight* light, Texture* diffuse, Texture* specualr, float shininess);
	void DrawMeshSpotLight(Mesh* mesh, const Matrix& model, SpotLight* light, Texture* diffuse, Texture* specular, float shininess);

	void DrawModel(Model* model);
	void DrawModelLightCaster(Model* model, LightCaster* light);
	void DrawModelDirectLight(Model* model, DirectionalLight* ligth);
	void DrawModelPointLight(Model* model, PointLight* light);
	void DrawModelSpotLight(Model* model, SpotLight* light);

private:
	Camera* camera;

	void PreDrawMesh(Shader* shader, Mesh* mesh, const Matrix& model);
	void PostDrawMesh(Mesh* mesh);

	FastDelegate2<int, int, void> window_resize_handle;
	void WindowResizeHandle(int width, int height);
};

}
