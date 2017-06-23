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
#include "Component.h"
#include "Transformable.h"

namespace cross{

class Entity : public Transformable {
public:
	Entity();
	~Entity();

	void Initialize();
	void Remove();
	void Update(float sec);
	void SetName(const string& name);
	const string& GetName();
	void AddComponent(Component* component);
	Component* GetComponent(Component::Type type);
	Entity* GetParent();
	void SetParent(Entity* parent);
	void AddChild(Entity* child);
	List<Entity*>& GetChildren();
	void RemoveChildren();
	Entity* FindChild(U32 index);
	Entity* FindChild(const string& name);
	Entity* RemoveChild(const string& nane);
	Entity* Clone();
	//Not optimized function
	Matrix GetWorldMatrix();
	Vector3D GetPosition();
	Vector3D GetDirection();

private:
	CROSS_FRIENDLY

	string name;
	Component* components[Component::COUNT];
	Entity* parent;
	List<Entity*> children;
};

}