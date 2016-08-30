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

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

namespace cross{

/* Loading 3D assets helper class */
class Graphics3D{
public:
	Graphics3D();
	~Graphics3D();

	/* Loads only one mesh from file */
	Mesh* LoadMesh(const string& filename);
	/* Loads whole file in Cross 3D structures */ 
	Model* LoadModel(const string& filename);

protected:
	const aiScene* current_scene;
	//fbx specific stuff
	Matrix current_geotranslation;
	Matrix current_translation;
	Matrix current_rotation;
	Matrix current_scaling;

	Mesh* ProcessNode(aiNode* node);
	void ProcessNode(Model* model, aiNode* node);
	Mesh* ProcessMesh(aiMesh* mesh);
	void LoadMeshes(Model* model);

	//Texture* LoadTexture(aiMaterial* material, unsigned int textureType, const string& modelFilePath);

};

}
