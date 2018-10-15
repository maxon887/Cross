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
#include "System.h"
#include "Component.h"
#include "Transform.h"
#include "Game.h"

using namespace cross;

Entity::Entity(const String& name) :
	name(name)
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

void Entity::SetName(const String& name) {
	this->name = name;
}

Component* Entity::GetComponent(U64 type) {
	return components[type];
}

Array<Component*> Entity::GetComponents() {
	Array<Component*> result;
	for(pair<U64, Component*> pair : components) {
		result.push_back(pair.second);
	}
	return result;
}

Transform* Entity::GetTransform() {
	CROSS_RETURN(HasComponent<Transform>(), nullptr, "Entity '#' does't contains Transform component", name);
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
	CROSS_FAIL(components.find(hash) == components.end(), "Entity already have same component");
	component->entity = this;
	components[hash] = component;
	if(initialize) {
		component->Initialize(scene);
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

Entity* Entity::FindChild(const String& name) {
	for(Entity* child : children) {
		if(child->GetName() == name) {
			return child;
		} else {
			child = child->FindChild(name);
			if(child) {
				return child;
			}
		}
	}
	return nullptr;
}

Entity* Entity::RemoveChild(const String& name) {
	for(auto it = children.begin(); it != children.end(); it++) {
		Entity* c = (*it);
		if(c->GetName() == name) {
			c->Remove();
			children.erase(it);
			return c;
		}
	}
	return nullptr;
}

Entity* Entity::RemoveChild(Entity* child) {
	for(auto it = children.begin(); it != children.end(); it++) {
		Entity* c = (*it);
		if(c == child) {
			c->Remove();
			children.erase(it);
			return c;
		}
	}
	return nullptr;
}

Entity* Entity::Clone() {
	Entity* clone = new Entity(this->name);
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

Matrix Entity::GetWorldMatrix() {
	if(parent){
		return parent->GetWorldMatrix() * GetTransform()->GetModelMatrix();
	}else{
		return GetTransform()->GetModelMatrix();
	}
}

Vector3D Entity::GetDirection() {
	if(parent) {
		return parent->GetTransform()->GetModelMatrix() * GetTransform()->GetDirection();
	} else {
		return GetTransform()->GetDirection();
	}
}

void Entity::Remove() {
	for(pair<U64, Component*> p : components) {
		Component* c = p.second;
		if(c) {
			c->Remove();
		}
	}
	for(Entity* c : children) {
		c->Remove();
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