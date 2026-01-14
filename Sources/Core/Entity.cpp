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

using namespace cross;
using namespace std;

Entity::Entity(String name) :
	name(std::move(name))
{ }

Entity::~Entity() {
	for(auto const& [id, component] : components) {
		component->Disable();
		delete component;
	}
	for(const Entity* c : children) {
		delete c;
	}
	children.clear();
}

const String& Entity::GetName() const {
	return name;
}

void Entity::SetName(String name) {
	this->name = std::move(name);
}

Component* Entity::GetComponent(U64 type) {
	auto result = components.find(type);
	if(result != components.end()) {
		return (*result).second;
	}
	return nullptr;
}

Array<Component*> Entity::GetComponents() {
	Array<Component*> result;
	for(const auto& [id, component] : components) {
		result.Add(component);
	}
	return result;
}

Transform* Entity::GetTransform() {
	CROSS_RETURN(HasComponent<Transform>(), nullptr, "Entity '#' doesn't contains Transform component", name);
	return GetComponent<Transform>();
}

void Entity::AddComponent(Component* component) {
	const U64 hash = typeid(*component).hash_code();
	if(components.find(hash) != components.end()) {
		CROSS_ASSERT(false, "Entity already have same component '#'", component->GetName());
		delete component;
		return;
	}
	component->entity = this;
	components[hash] = component;
	if(on_scene) {
		component->Enable();
	}
}

void Entity::RemoveComponent(Component* component) {
	component->Disable();
	components.erase(typeid(*component).hash_code());
}

Entity* Entity::GetParent() const {
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
	child->SetOnScene(this->on_scene);
}

void Entity::RemoveChildren() {
	for(const Entity* c : children) {
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

Entity* Entity::FindChild(const String& childName) const {
	for(Entity* child : children) {
		if(child->GetName() == childName) {
			return child;
		}
		child = child->FindChild(childName);
		if(child) {
			return child;
		}
	}
	return nullptr;
}

Entity* Entity::RemoveChild(const String& childName) {
	for(auto it = children.begin(); it != children.end(); ++it) {
		Entity* c = *it;
		if(c->GetName() == childName) {
			c->SetParent(nullptr);
			children.erase(it);
			c->SetOnScene(false);
			return c;
		}
	}
	CROSS_RETURN(false, nullptr, "Unable to find child Entity '#'", childName);
}

Entity* Entity::RemoveChild(Entity* child) {
	CROSS_RETURN(child, nullptr, "Trying to remove null pointer");
	for(auto it = children.begin(); it != children.end(); ++it) {
		Entity* c = *it;
		if(c == child) {
			c->SetParent(nullptr);
			children.erase(it);
			c->SetOnScene(false);
			return c;
		}
	}
	CROSS_RETURN(false, nullptr, "Unable to find child Entity '#'", child->GetName());
}

Entity* Entity::Clone() {
	Entity* clone = CREATE Entity(this->name);
	for(const auto& [id, component] : components){
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

bool Entity::IsOnScene() const {
	return on_scene;
}

Vector3D Entity::GetDirection() {
	if(parent) {
		return parent->GetTransform()->GetModelMatrix() * GetTransform()->GetDirection();
	}
	return GetTransform()->GetDirection();
}

void Entity::SetOnScene(bool onScene) {
	if(on_scene != onScene) {
		on_scene = onScene;
		for(const auto& [ID, component] : components) {
			if(onScene && component->IsActive()) {
				component->Enable();
			} else {
				component->Disable();
			}
		}
	}
	for(Entity* c : children) {
		c->SetOnScene(onScene);
	}
}

void Entity::Update(float sec) {
	for(const auto& [ID, component] : components) {
		if(component->IsActive()) {
			component->Update(sec);
		}
	}
	for(Entity* c : children) {
		c->Update(sec);
	}
}