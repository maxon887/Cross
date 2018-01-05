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
#include "Screen.h"
#include "Input.h"
#include "Model.h"

namespace tinyxml2{

class XMLElement;
class XMLDocument;

}

namespace cross{

/*	Scene is place where all Entities live. Scene manages Entity live time, update cycles, utilization.
	Second main task of this class is resource management.Textures, Models, Shaders, Materials also stored there. 
	You can obtain requared resource by calling Get[ResourceName]() function.
	You don't need to call Entity's or resource destructors manually */
class Scene : public Screen {
public:
	/* Occurs when new Entity added into the Scene */
	Event<Entity*> EntityAdded;

	Scene();
	Scene(const string& filename);

	/* Called once before scene show up. */
	virtual void Start() override;
	/* Called once when scene about to change on new one */
	virtual void Stop() override;
	/* Called every frame update. */
	virtual void Update(float sec) override;

	/* Loads scene from file(.scn). Returns true if succeed */
	bool Load(const string& file);
	/* Save scene into file */
	void Save(const string& file);
	/* Removes all loaded stuff from scene (Textures, Materials, Shader, Models and Entities) */
	void Clear();
	/* Returns root Scene Entity */
	Entity* GetRoot();
	/* Returns scene main 3D camera */
	Camera* GetCamera();
	/* Sets main 3D camera for this Scene */
	void SetCamera(Camera* cam);
	/* Finds specific entity on this Scene by name */
	Entity* GetEntity(const string& name);
	/* Adds an entity into the Scene */
	void AddEntity(Entity* entity);
	/* Loads primitive model as Entity with Mesh Component included */
	Entity* LoadPrimitive(Model::Primitive primitive);
	/* Removes entity from this Scene by name. Returns founded Entity or null otherwise. Entity won't we destructed */
	Entity* RemoveEntity(const string& name);
	/* Returns all available Lights on the Scene. */
	List<Light*>& GetLights();
	/* Sets up ambient Scene Light Color */
	void SetAmbientColor(const Color& color);
	/* Returns ambient Scene Light Color */
	Color GetAmbientColor() const;
	/* Obtain loaded into scene Shader or load it by self in other way */
	Shader* GetShader(const string& shaderfile);
	/* Obtain loaded into scene Material or load it by self in other way */
	Material* GetMaterial(const string& xmlFile);
	/* Obtain loaded into scene Texture or load it by self in other way */
	Texture* GetTexture(const string& textureFile);
	/* Obtain loaded into scene Model or load it by self in other way */
	Model* GetModel(const string& modelFile);
	/* Resets all materials to default state */
	void ResetMaterials();

protected:
	Dictionary<U32, Shader*> shaders		= Dictionary<U32, Shader*>();
	Dictionary<U32, Texture*> textures		= Dictionary<U32, Texture*>();
	Dictionary<U32, Material*> materials	= Dictionary<U32, Material*>();
	Dictionary<U32, Model*> models			= Dictionary<U32, Model*>();
	List<Light*> lights						= List<Light*>();
	Entity* root							= NULL;
	string filename							= "";

	Camera* camera							= NULL;
	Color ambient_color						= Color(0.1f, 0.1f, 0.1f);

private:
	static const U32 scene_loader_version	= 16;
	static const U32 scene_saver_version	= 15;

	bool LoadEntity(Entity* parent, tinyxml2::XMLElement* xml);
	bool SaveEntity(Entity* e, tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc);

	void OnWindowResize(S32 width, S32 height);

	void CreateDefaultCamera();
};

}
