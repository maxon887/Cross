/*	Copyright © 2018 Maksim Lukyanov

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
#include "Base/Array.h"
#include "Base/String.h"
#include "Base/List.h"
#include "Base/Map.h"
#include "Math/Vector3D.h"

#include <typeinfo>

namespace cross{

/*	Class representing base game object. Usually Entities used as containers for Components.
	Almost any object exiting on a scene must be an Entity. All Entities stores in Scene in the Tree-like structure */
class Entity {
public:
	Entity(String name);
	~Entity();

	/* Returns name of the object. Entity can be found by name in Scene or in other Entity's children by GetEntity() or FindChild() */
	const String& GetName() const;
	/* Sets name of the object. Rewrites name given by constructor */
	void SetName(String name);

	/* Checks if Entity contains certain component T */
	template<class T> bool HasComponent() const;
	/* Returns certain component T contained in this Entity or nullptr if component not found */
	template<class T> T* GetComponent();
	/* Returns component by id contained in this Entity or nullptr if component not found */
	Component* GetComponent(U64 type);
	/* Returns all components contained in this Entity */
	Array<Component*> GetComponents();
	/* Returns Transform component contained in this Entity or nullptr if Transform not found */
	Transform* GetTransform();
	/* Adds component to the current Entity component stack. Duplicated components not allowed. In case of error input component will be force deleted */
	void AddComponent(Component* component);
	/* Removes component from Entity. Appropriate Remove() will be called on Component object */
	void RemoveComponent(Component* component);

	/* Returns parent of the Entity or nullptr if Entity doesn't have a parent (for root entity for ex) */
	Entity* GetParent() const;
	/* Sets parent for this Entity */
	void SetParent(Entity* parent);
	/* Returns all children held by this Entity */
	List<Entity*>& GetChildren();
	/* Adds child to the Entity. Entity can hold as many children as you need. All children will be initialized and updated properly */
	void AddChild(Entity* child);
	/* Removes all children held by this Entity */
	void RemoveChildren();
	/* Returns Entity's child by its index position in Entity's child container */
	Entity* FindChild(U32 index);
	/* Returns Entity's child by its name */
	Entity* FindChild(const String& childName) const;
	/* Removes child from Entity by name. Returns live child in case of success or nullptr if child not found. Returned child must be utilized by hand. */
	Entity* RemoveChild(const String& childName);
	/* Removes specific child from Entity. Returns the same object in case of success or nullptr if child not found. Appropriate child's Remove() will be called. */
	Entity* RemoveChild(Entity* child);
	/* Clone this entity with all it's components and children */
	Entity* Clone();
	/* Check whether Entity is on active Scene */
	bool IsOnScene() const;
	/* Returns Entity's world direction vector. Not fast function (all parents directions must be multiplied and must exist) */
	Vector3D GetDirection();

engineonly:
	void SetOnScene(bool onScene);
	void Update(float sec);
	List<Entity*> children					= List<Entity*>();

private:
	String name								= String();
	Map<U64, Component*> components			= Map<U64, Component*>();
	Entity* parent							= nullptr;
	bool on_scene							= false;
};

template<class T>
bool Entity::HasComponent() const {
	return components.find(typeid(T).hash_code()) != components.end();
}

template<class T>
T* Entity::GetComponent() {
	const size_t typeID = typeid(T).hash_code();
	return (T*)GetComponent(typeID);
}

}