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
#include "Entity.h"
#include "Graphics3D/CameraControlsScene.h"
#include "Shaders/MultiLightShader.h"
#include "Physics/Physics.h"

class CableConstraint : public Entity,
						public CollisionProvider {
public:
	CableConstraint(float length, Vector3D anchor, Collider* connectedObject);

	virtual void Update(float sec);
	virtual void Provide(Array<Collision>& collisions, Array<Collider*>& colliders);
	
private:
	float length;
	Vector3D anchor;
	Collider* object;
};

class Cable : public Entity,
			  public CollisionProvider {
public:
	Cable(Collider* a, Collider* b);

	virtual void Update(float sec);
	virtual void Provide(Array<Collision>& collisions, Array<Collider*>& colliders);

	void SetLength(float len);

private:
	float length;
	Collider* endA;
	Collider* endB;
};

class Rod : public Entity,
			public CollisionProvider {
public:
	Rod(Collider* a, Collider* b);

	virtual void Update(float sec);
	virtual void Provide(Array<Collision>& collision, Array<Collider*>& colliders);

	Collider* GetEndA();
	Collider* GetEndB();

private:
	float length;
	Collider* endA;
	Collider* endB;
};

class HardConstraints : public CameraControlsScene,
						public CollisionProvider {
public:
	virtual void Start();
	virtual void Stop();
	virtual void Update(float sec);

	virtual void Provide(Array<Collision>& collisions, Array<Collider*>& colliders);

private:
	MultiLightShader* road_shader;
	Texture* road_diffuse;
	Material* road_mat;

	Shader* particle_shader;
	Material* particle_mat;

	Entity* rodA;
	Entity* rodB;
};