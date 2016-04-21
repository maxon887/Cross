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

class Graphics3D{
public:
	Graphics3D();
	~Graphics3D();

	Camera* GetCamera();
	void AddLightSource(Light* light);
	void ClearLightSources();

	Model* LoadModel(Shader::Type shaderType, const string& filename);
	Mesh* LoadMesh(const string& filename);
	CRArray<Mesh*> LoadMeshes(const string& filename);

	void DrawMeshSimple(Mesh* mesh, const Matrix& model, Color& color);
	void DrawMeshTexture(Mesh* mesh, const Matrix& model, Texture* diffuse);
	void DrawMeshLightMaterial(Mesh* mesh, const Matrix& model);
	void DrawMeshLightMaps(Mesh* mesh, const Matrix& model, Texture* diffuse, Texture* specular);

private:
	Camera* camera;
	CRArray<Light*> light_sources;

	FastDelegate2<int, int, void> window_resize_handle;
	void WindowResizeHandle(int width, int height);
};

}
