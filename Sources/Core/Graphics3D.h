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

class MultiLightShader;

/*	Class responsible for 3D objects drawing 
	and model loading */
class Graphics3D{
public:
	enum Primitives{
		CUBE,
		SPHERE,
		PLANE,
		COUNT
	};

	enum StencilBehaviour{
		WRITE,
		READ,
		IGNORED
	};

	Graphics3D();
	~Graphics3D();

	void DrawLine(const Vector3D& p1, const Vector3D& p2, const Color& c);
	Entity* LoadPrimitive(Primitives primitive);
	Entity* LoadModel(const string& filename, bool initialize = true);
	void AdjustMaterial(Entity* model, Material* material, bool faceCulling = true);
	Material* GetDefaultMaterial();

protected:
	Shader* simple_shader				= NULL;
	MultiLightShader* default_shader	= NULL;
	Material* default_material			= NULL;
	const aiScene* current_scene		= NULL;
	bool initialize_in_load				= true;
	Array<Entity*> primitives			= Array<Entity*>(COUNT, NULL);

	void ProcessScene(Entity* model, const string& filename);
	void ProcessNode(Entity* model, aiNode* node);
	Mesh* ProcessMesh(aiMesh* mesh);

};

}
