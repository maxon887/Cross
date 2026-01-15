#pragma once
#include "Cross.h"
#include "Base/Map.h"

struct aiMesh;
struct aiNode;
struct aiScene;

namespace cross{

/*	Model is a hierarchy of Entities with Mesh Components. Hierarchy and Mesh data usually loaded from model file.
	Model lifetime managed by Scene. To load Model you need to use Scene::GetModel(). Duplicated models won't be loaded */
class Model {
public:
	/* Primitives model. Can be loaded by engine without a file */
	enum Primitive {
		CUBE,
		SPHERE,
		PLANE
	};

	~Model();

	/* Loads Model from file. Used by engine. Use Scene::GetModel */
	bool Load(const String& filename);
	/* Loads Model from file and provides tangents data for model if needed */
	bool Load(const String& filename, bool calcTangents);
	/* Loads Model, provides tangents data for model and can transfer model data to video memory */
	bool Load(const String& filename, bool calcTangents, bool initializeVideoData);
	/* Returns model's object hierarchy as Entity hierarchy */
	Entity* GetHierarchy() const;
	/* Returns specific Mesh Components from model by id */
	Mesh* GetMesh(const String& id);

private:
	Map<String, Mesh*> meshes;
	Entity* hierarchy;

	const aiScene* current_scene	= nullptr;
	bool initialize_video = true;

	bool ProcessScene(Entity* root, File* sceneFile, bool calcTangents, const String& filename);
	void ProcessNode(Entity* entity, aiNode* node, const String& filename);
	Mesh* ProcessMesh(aiMesh* mesh, const String& filename, const String& groupID);
};

}