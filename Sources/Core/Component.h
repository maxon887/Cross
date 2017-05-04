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
#include "Transformable.h"

namespace cross{

class Component{
public:
	enum Type{
		MESH,
		LIGHT,
		RIGIDBODY,
		COLLIDER,
		COUNT
	};

	Component(Type type);
	virtual ~Component() { };

	virtual void Update(float sec);
	virtual Component* Clone();

	void Initialize(Entity* entity);
	bool Initialized() const;
	Type GetType() const;
	Entity* GetEntity();
	Component* GetComponent(Component::Type type);
	Transformable* GetTransform();
	Vector3D GetPosition();
	void SetPosition(const Vector3D& pos);

private:
	Type type;
	Entity* entity;
};

}