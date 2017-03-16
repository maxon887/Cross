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
	/* Returns scene 3D camera */
	Camera* GetCamera();
	/* Reset camera view distance. Can affect on performance */
	void SetCameraViewDistance(float distance);
	/* Adds new light to the scene */
	void AddLight(Light* light);
	void AddEntity(Entity* entity);
	/* Returns all avalible light on scne */
	Array<Light*>& GetLights();
	/* Sets up ambient scene light intencity */
	void SetAmbientColor(const Color& color);
	/* Returns ambient scene light intencity */
	Color GetAmbientColor() const;

protected:
	Camera* camera;
	Color ambient_color;

	Array<Light*> lights;
	Array<Entity*> objects;
	Array<Collider*> colliders;

private:
	FastDelegate2<S32, S32, void> window_resize_handle;
	void WindowResizeHandle(S32 width, S32 height);
	void ProcessCollisions();
};

}