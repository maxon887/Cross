/*	Copyright © 2018 Maksim Lukyanov

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

struct aiMesh;
struct aiNode;
struct aiScene;

namespace cross{

/*	Model is a hierarchy of Entities with Mesh Components. Hierarchy and Mesh data usually loaded from model file.
	Model life time managed by Scene. To load Model you need to use Scene::GetModel(). Dublicated models won't be loaded */
class Model {
public:
	/* Primitives model. Can be loaded by engine without a file */
	enum Primitive {
		CUBE,
		SPHERE,
		PLANE,
		COUNT
	};

	~Model();

	/* Loads Model from file. Used by engine. Use Scene::GetModel*/
	void Load(const String& filename, bool initialize = true);
	/* Returns model's filename if was loaded from file */
	const String& GetFilename() const;
	/* Returns model's object hierarchy as Entity hierarchy */
	Entity* GetHierarchy() const;
	/* Returns specific Mesh Components from model by id */
	Mesh* GetMesh(S32 id);

private:
	String filename;
	Dictionary<S32, Mesh*> meshes;
	Entity* hierarchy;

	const aiScene* current_scene	= nullptr;
	bool initialize_in_load			= true;
	S32 mesh_id						= 0;

	void ProcessScene(Entity* root, File* sceneFile);
	void ProcessNode(Entity* entity, aiNode* node);
	Mesh* ProcessMesh(aiMesh* mesh);
};

}