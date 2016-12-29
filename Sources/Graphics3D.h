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

/*	Class responsible for 3D objetcs drawing 
	and model loading */
class Graphics3D{
public:
	enum Primitives{
		CUBE,
		SPHERE,
		PLANE
	};

	enum StencilBehaviour{
		WRITE,
		READ,
		IGNORED
	};

	Graphics3D();
	~Graphics3D();

	Model* LoadPrimitive(Primitives primitive);
	Model* LoadModel(const string& filename);

	void DrawMesh(Mesh* mesh, const Matrix& model, bool faseCulling, bool alphaBlending);
	void DrawMesh(Mesh* mesh, const Matrix& model, bool faceCulling, bool alphaBlending, StencilBehaviour stencilBehvaiour);

protected:
	const aiScene* current_scene;
	//fbx specific stuff
	Matrix current_geotranslation;
	Matrix current_translation;
	Matrix current_pre_rotation;
	Matrix current_rotation;
	Matrix current_scaling;

	void ProcessScene(Model* model);
	void ProcessNode(Model* model, aiNode* node);
	Mesh* ProcessMesh(aiMesh* mesh);

};

}
