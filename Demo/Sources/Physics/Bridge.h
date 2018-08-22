/*	Copyright � 2015 Lukyanau Maksim

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
#include "Graphics3D/DemoScene.h"
#include "Physics/HardConstraints.h"

class Bridge : public DemoScene {
public:
	virtual void Start();
	virtual void Stop();
	virtual void Update(float sec);

private:
	Shader* particle_shader;
	Material* particle_mat;
	Material* mass_mat;

	Array<Entity*> nodes;
	Entity* mass;
	Vector3D mass_pos;

	Rod* CreateRod(RigidBody* a, RigidBody* b);
	CableConstraint* CreateCable(float len, Vector3D& ancor, RigidBody* obj);
	Cable* Connect(RigidBody* a, RigidBody* b);
};