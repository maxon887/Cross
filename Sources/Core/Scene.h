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
	Scene();
	/* Called before scene show up. */
	virtual void Start();
	/* Called when scene about to change on new one */
	virtual void Stop();
	/* Called every frame update. */
	virtual void Update(float sec);

	void Load(const string& file);
	void Save(const string& file);
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
	/* Returns all game objects on scene */
	List<Entity*>& GetObjects();
	/* Returns all available light on scene. */
	List<Light*>& GetLights();
	/* Sets up ambient scene light intensity */
	void SetAmbientColor(const Color& color);
	/* Returns ambient scene light intensity */
	Color GetAmbientColor() const;

protected:
	Dictionary<U32, Shader*> shaders;
	Dictionary<U32, Texture*> textures;
	Dictionary<U32, Material*> materials;
	List<Entity*> objects;
	List<Light*> lights;

	Camera* camera;
	Color ambient_color;

private:
	string name;
    U64 resize_del;
	void WindowResizeHandle(S32 width, S32 height);
};

}