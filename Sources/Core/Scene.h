#pragma once
#include "Cross.h"
#include "Screen.h"
#include "Base/List.h"
#include "Model.h"
#include "Texture.h"
#include "Color.h"

namespace tinyxml2{

class XMLElement;
class XMLDocument;

}

namespace cross{

/*	Scene is place where all Entities live. Scene manages Entity live time, update cycles, utilization.
	Second main task of this class is resource management.Textures, Models, Shaders, Materials also stored there.
	You can obtain required resource by calling Get[ResourceName]() function.
	You don't need to call Entity's or resource destructors manually */
class Scene : public Screen {
public:
	Scene();
	~Scene() override;

	/* Called once before scene show up. */
	void Start() override;
	/* Called once when scene about to change on new one */
	void Stop() override;
	/* Called every frame update. */
	void Update(float sec) override;

	/* Loads scene from file(.scn). Returns true if succeed */
	virtual bool Load(const String& filename);
	/* Save scene into file */
	virtual void Save(const String& filename);

	/* Removes all loaded stuff from scene (Textures, Materials, Shader, Models and Entities) */
	void Clear();
	/* Returns root Scene Entity */
	Entity* GetRoot();
	/* Returns scene main 3D camera */
	Camera* GetCamera();
	/* Sets main 3D camera for this Scene */
	void SetCamera(Camera* cam);
	/* Finds specific entity on this Scene by name */
	Entity* GetEntity(const String& name);
	/* Adds an entity into the Scene */
	void AddEntity(Entity* entity);
	/* Loads primitive model as Entity with Mesh Component included */
	Entity* LoadPrimitive(Model::Primitive primitive);
	/* Removes entity from Scene by name. Returns founded Entity or nullptr otherwise. Entity won't be destructed */
	Entity* RemoveEntity(const String& name);
	/* Returns all available Lights on the Scene. */
	List<Light*>& GetLights();
	/* Sets up ambient Scene Light Color */
	void SetAmbientColor(const Color& color);
	/* Returns ambient Scene Light Color */
	Color GetAmbientColor() const;
	/* Obtain loaded into scene Shader or load it by self in other way */
	Shader* GetShader(const String& shaderFile);
	/* Obtain loaded into scene Material or load it by self in other way */
	Material* GetMaterial(const String& xmlFile);
	/* Returns default engine material */
	Material* GetDefaultMaterial();
	/* Obtain loaded into scene Texture or load it by self in other way */
	Texture* GetTexture(const String& textureFile);
	/* Loads texture with provided filter */
	Texture* GetTexture(const String& textureFile, Texture::Filter filter);
	/* Obtain loaded into scene Model or load it by self in other way */
	Model* GetModel(const String& modelFile, bool calcTangents = false);

protected:
	Entity* root							= nullptr;

	Map<U64, Shader*> shaders				= Map<U64, Shader*>();
	Map<U64, Texture*> textures				= Map<U64, Texture*>();
	Map<U64, Material*> materials			= Map<U64, Material*>();
	Map<U64, Model*> models					= Map<U64, Model*>();
	List<Light*> lights						= List<Light*>();

	Camera* camera							= nullptr;
	Color ambient_color						= Color(0.1f, 0.1f, 0.1f);

	void CreateDefaultCamera();

private:
	static const U32 scene_loader_version	= 21;
	static const U32 scene_saver_version	= 21;

	bool LoadEntity(Entity* parent, tinyxml2::XMLElement* xml);
	bool SaveEntity(Entity* e, tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc);

	void OnWindowResize(S32 width, S32 height);
};

}
