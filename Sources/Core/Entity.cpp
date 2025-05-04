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
#include "Entity.h"
#include "Component.h"
#include "Transform.h"
#include "Game.h"

using namespace cross;
using namespace std;

Entity::Entity(String name) :
	name(std::move(name))
{ }

Entity::~Entity() {
	for(pair<U64, Component*> p : components) {
		Component* component = p.second;
		if(component) {
			component->Remove();
			delete component;
		}
	}
	for(Entity* c : children) {
		delete c;
	}
	children.clear();
}

void Entity::Initialize() {
	for(Entity* c : children) {
		c->Initialize();
	}
}

const String& Entity::GetName() const {
	return name;
}

void Entity::SetName(String name) {
	this->name = std::move(name);
}

Component* Entity::GetComponent(U64 type) {
	if(components.find(type) == components.end()) {
		return nullptr;
	} else {
		return components[type];
	}
}

Array<Component*> Entity::GetComponents() {
	Array<Component*> result;
	for(pair<U64, Component*> pair : components) {
		result.Add(pair.second);
	}
	return result;
}

Transform* Entity::GetTransform() {
	CROSS_RETURN(HasComponent<Transform>(), nullptr, "Entity '#' doesn't contains Transform component", name);
	return GetComponent<Transform>();
}

void Entity::AddComponent(Component* component) {
	AddComponent(component, game->GetCurrentScene());
}

void Entity::AddComponent(Component* component, Scene* scene) {
	AddComponent(component, scene, true);
}

void Entity::AddComponent(Component* component, Scene* scene, bool initialize) {
	U64 hash = typeid(*component).hash_code();
	if(components.find(hash) != components.end()) {
		CROSS_ASSERT(false, "Entity already have same component '#'", component->GetName());
		delete component;
		return;
	}
	component->entity = this;
	if(initialize) {
		if(component->Initialize(scene)) {
			components[hash] = component;
		} else {
			CROSS_ASSERT(false, "Component '#' was not properly initialized. It can not be added to Entity", component->GetName());
			delete component;
			return;
		}
	} else {
		components[hash] = component;
	}
}

void Entity::RemoveComponent(Component* component) {
	component->Remove();
	components.erase(typeid(*component).hash_code());
}

Entity* Entity::GetParent() {
	return parent;
}

void Entity::SetParent(Entity* p) {
	parent = p;
}

List<Entity*>& Entity::GetChildren() {
	return children;
}

void Entity::AddChild(Entity* child) {
	child->SetParent(this);
	children.push_back(child);
}

void Entity::RemoveChildren() {
	for(Entity* c : children) {
		delete c;
	}
	children.clear();
}

Entity* Entity::FindChild(U32 index) {
	CROSS_RETURN(index < children.size(), nullptr, "Out of bounds");
	auto it = children.begin();
	std::advance(it, index);
	return *it;
}

Entity* Entity::FindChild(const String& childName) {
	for(Entity* child : children) {
		if(child->GetName() == childName) {
			return child;
		} else {
			child = child->FindChild(childName);
			if(child) {
				return child;
			}
		}
	}
	return nullptr;
}

Entity* Entity::RemoveChild(const String& childName) {
	for(auto it = children.begin(); it != children.end(); it++) {
		Entity* c = (*it);
		if(c->GetName() == childName) {
			c->SetParent(nullptr);
			children.erase(it);
			return c;
		}
	}
	CROSS_RETURN(false, nullptr, "Unable to find child Entity '#'", childName);
}

Entity* Entity::RemoveChild(Entity* child) {
	CROSS_RETURN(child, nullptr, "Trying to remove null pointer");
	for(auto it = children.begin(); it != children.end(); it++) {
		Entity* c = (*it);
		if(c == child) {
			c->SetParent(nullptr);
			children.erase(it);
			return c;
		}
	}
	CROSS_RETURN(false, nullptr, "Unable to find child Entity '#'", child->GetName());
}

Entity* Entity::Clone() {
	Entity* clone = CREATE Entity(this->name);
	for(pair<U64, Component*> pair : components){
		Component* component = pair.second;
		clone->components[typeid(*component).hash_code()] = component->Clone();
		clone->components[typeid(*component).hash_code()]->entity = clone;
	}
	for(Entity* child : children){
		Entity* cloneChild = child->Clone();
		cloneChild->parent = clone;
		clone->AddChild(cloneChild);
	}
	return clone;
}

Vector3D Entity::GetDirection() {
	if(parent) {
		return parent->GetTransform()->GetModelMatrix() * GetTransform()->GetDirection();
	} else {
		return GetTransform()->GetDirection();
	}
}

void Entity::Update(float sec) {
	for(pair<U64, Component*> p : components) {
		Component* c = p.second;
		if(c->IsEnabled()) {
			c->Update(sec);
		}
	}
	for(Entity* c : children) {
		c->Update(sec);
	}
}