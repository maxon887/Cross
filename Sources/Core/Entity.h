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

#include <typeinfo>

namespace cross{

class Entity {
public:
	Entity(const string& name);
	~Entity();

	void Initialize();
	void Remove();
	void Update(float sec);
	void SetName(const string& name);
	const string& GetName() const;
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);
	template<class T> T* GetComponent();
	Component* GetComponent(U64 type);
	Transform* GetTransform();
	Entity* GetParent();
	void SetParent(Entity* parent);
	void AddChild(Entity* child);
	List<Entity*>& GetChildren();
	void RemoveChildren();
	Entity* FindChild(U32 index);
	Entity* FindChild(const string& name);
	Entity* RemoveChild(const string& nane);
	Entity* RemoveChild(Entity* child);
	Entity* Clone();
	//Not optimized function
	Matrix GetWorldMatrix();
	Vector3D GetDirection();

private:
	CROSS_FRIENDLY

	string name								= string();
	Dictionary<U64, Component*> components	= Dictionary<U64, Component*>();
	Entity* parent							= NULL;
	List<Entity*> children					= List<Entity*>();
};

template<class T>
T* Entity::GetComponent(){
	auto it = components.find(typeid(T).hash_code());
	if(it != components.end()){
		return (T*)(*it).second;
	}else{
		return NULL;
	}
}

}