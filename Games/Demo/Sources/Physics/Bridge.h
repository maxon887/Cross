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
#include "Graphics3D/CameraControlsScene.h"
#include "Physics/HardConstraints.h"

class Bridge : public CameraControlsScene {
public:
	virtual void Start();
	virtual void Stop();
	virtual void Update(float sec);

private:
	Shader* particle_shader;
	Material* particle_mat;
	Array<Rod*> rods;
	Array<Cable*> cables;

	Rod* test;

	Rod* CreateRod(Vector3D& a, Vector3D& b);
	Cable* CreateCable(float len, Vector3D& ancor, Collider* obj);
	Rod* Connect(Collider* a, Collider* b);
};