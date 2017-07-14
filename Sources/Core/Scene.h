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
#include "Screen.h"
#include "Input.h"

namespace cross{

/*	This class designed for managing 3D space. 
	Models, Meshes, Lights can exists only in Scene */
class Scene : public Screen{
public:
	Event<Entity*> EntityAdded;
	/* Called before scene show up. */
	virtual void Start() override;
	/* Called when scene about to change on new one */
	virtual void Stop() override;
	/* Called every frame update. */
	virtual void Update(float sec) override;

	const string& GetName() const;
	void SetName(const string& name);
	/* Loads scene from file(.scn) */
	void Load(const string& file, bool absolutePath = false);
	/* Loads scene in to file */
	void Save(const string& file);
	void Clear();
	/* Returns root scene entity*/
	Entity* GetRoot();
	/* Returns scene 3D camera */
	Camera* GetCamera();
	/* Reset camera view distance. Can affect on performance */
	void SetCameraViewDistance(float distance);
	/* Finds specific entity on the scene by name */
	Entity* GetEntity(const string& name);
	/* Adds entity object to the scene */
	void AddEntity(Entity* entity);
	/* Removes entity from scene by name */
	Entity* RemoveEntity(const string& name);
	/* Returns all available light on scene. */
	List<Light*>& GetLights();
	/* Sets up ambient scene light intensity */
	void SetAmbientColor(const Color& color);
	/* Returns ambient scene light intensity */
	Color GetAmbientColor() const;

protected:
	Dictionary<S32, Shader*> shaders		= Dictionary<S32, Shader*>();
	Dictionary<S32, Texture*> textures		= Dictionary<S32, Texture*>();
	Dictionary<S32, Material*> materials	= Dictionary<S32, Material*>();
	List<Light*> lights						= List<Light*>();
	Entity* root							= NULL;

	Camera* camera							= NULL;
	Color ambient_color						= Color(0.1f, 0.1f, 0.1f);

	S32 FindShaderID(Shader* shader);
	S32 FindTextureID(Texture* texture);

private:
	static const U32 scene_loader_version	= 11;
	string name								= "";
    U64 resize_del							= -1;
	void WindowResizeHandle(S32 width, S32 height);
};

}