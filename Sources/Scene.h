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

class Scene : public Screen{
public:
	Scene();

	virtual void Start();
	virtual void Update(float sec);
	virtual void Stop();

	void SetCameraViewDistance(float distance);
	void AddLight(Light* light);
	Array<Light*>& GetLights();

	Camera* GetCamera();
	Color GetAmbientColor() const;
	void SetAmbientColor(const Color& color);

protected:
	Camera* camera;
	Color ambient_color;

	Array<Light*> lights;
	Array<Model*> models;

private:
	FastDelegate2<S32, S32, void> window_resize_handle;
	void WindowResizeHandle(S32 width, S32 height);
};

}