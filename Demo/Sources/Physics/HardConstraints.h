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
#include "Graphics3D/DemoScene.h"
#include "Entity.h"
#include "Physics/Physics.h"
#include "Shaders/LightsShader.h"

using namespace cross;

class Connection{
public:
	Connection();

	void SetColor(const Color& c);

protected:
	Color color;
};

class CableConstraint : public Entity,
						public Connection,
						public CollisionProvider {
public:
	CableConstraint(float length, Vector3D anchor, RigidBody* body);

	virtual void Update(float sec);
	virtual void Provide(Array<Collision>& collisions, Array<Collider*>& colliders);
	
private:
	float length;
	Vector3D anchor;
	RigidBody* body;
};

class Cable : public Entity,
			  public Connection,
			  public CollisionProvider {
public:
	Cable(RigidBody* a, RigidBody* b);

	virtual void Update(float sec);
	virtual void Provide(Array<Collision>& collisions, Array<Collider*>& colliders);

	void SetLength(float len);

private:
	float length;
	RigidBody* endA;
	RigidBody* endB;
};

class Rod : public Entity,
			public Connection,
			public CollisionProvider {
public:
	Rod(RigidBody* a, RigidBody* b);

	virtual void Update(float sec);
	virtual void Provide(Array<Collision>& collision, Array<Collider*>& colliders);

	RigidBody* GetEndA();
	RigidBody* GetEndB();

private:
	float length;
	RigidBody* endA;
	RigidBody* endB;
};

class HardConstraints : public DemoScene,
						public CollisionProvider {
public:
	virtual void Start();
	virtual void Stop();
	virtual void Update(float sec);

	virtual void Provide(Array<Collision>& collisions, Array<Collider*>& colliders);

private:
	Shader* road_shader;
	Texture* road_diffuse;
	Material* road_mat;

	Shader* particle_shader;
	Material* particle_mat;

	Entity* rodA;
	Entity* rodB;
};