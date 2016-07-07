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

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

namespace cross{

class Graphics3D{
public:
	Graphics3D();
	~Graphics3D();

	Mesh* LoadMesh(const string& filename);
	Model* LoadModel(const string& filename);

private:
	const aiScene* current_scene;

	Mesh* ProcessNode(aiNode* node);
	Mesh* ProcessMesh(aiMesh* mesh);

	CRArray<Texture*>* LoadTextures(aiMaterial* material, unsigned int textureType, const string& modelFilePath);
	Texture* LoadTexture(aiMaterial* material, unsigned int textureTepy, const string& modelFilePath);
	void ProcessNode(Model* model, aiNode* node);
	//old stuff
	void LoadMeshes(Model* model);

	Model* LoadModel(const string& filename, Shader* shader);
};

}
