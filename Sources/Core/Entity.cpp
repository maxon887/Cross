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
#include "Entity.h"
#include "System.h"
#include "Component.h"
#include "Transform.h"

using namespace cross;

Entity::Entity(const string& name) :
	name(name)
{ }

Entity::~Entity(){
	for(pair<U64, Component*> p : components) {
		Component* component = p.second;
		if(component) {
			component->Remove();
			delete component;
		}
	}
	for(Entity* c : children){
		delete c;
	}
	children.clear();
}

void Entity::Initialize(){
	for(Entity* c : children){
		c->Initialize();
	}
}

void Entity::Remove(){
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

void Entity::Update(float sec){
	for(pair<U64, Component*> p : components) {
		Component* c = p.second;
		if(c->IsEnabled()){
			c->Update(sec);
		}
	}
	for(Entity* c : children){
		c->Update(sec);
	}
}

void Entity::SetName(const string& name){
	this->name = name;
}

const string& Entity::GetName() const{
	return name;
}

void Entity::AddComponent(Component* component){
	component->entity = this;
	components[typeid(*component).hash_code()] = component;
	component->Initialize();
}

void Entity::RemoveComponent(Component* component){
	component->Remove();
	components.erase(typeid(*component).hash_code());
}

Component* Entity::GetComponent(U64 type) {
	return components[type];
}

Transform* Entity::GetTransform() {
	CROSS_RETURN(components.find(typeid(Transform).hash_code()) != components.end(), NULL, 
		"Entity %s does't contains Tranform component", name.c_str());
	return GetComponent<Transform>();
}

Entity* Entity::GetParent(){
	return parent;
}

void Entity::SetParent(Entity* p){
	parent = p;
}

void Entity::AddChild(Entity* child){
	child->SetParent(this);
	children.push_back(child);
}

List<Entity*>& Entity::GetChildren(){
	return children;
}

void Entity::RemoveChildren(){
	for(Entity* c : children) {
		delete c;
	}
	children.clear();
}

Entity* Entity::FindChild(U32 index){
	CROSS_RETURN(index < children.size(), NULL, "Out of bounds");
	auto it = children.begin();
	std::advance(it, index);
	return *it;
}

Entity* Entity::FindChild(const string& name){
	for(Entity* child : children){
		if(child->GetName() == name){
			return child;
		}else{
			child = child->FindChild(name);
			if(child){
				return child;
			}
		}
	}
	return NULL;
}

Entity* Entity::RemoveChild(const string& name){
	for(auto it = children.begin(); it != children.end(); it++){
		Entity* c = (*it);
		if(c->GetName() == name){
			c->Remove();
			children.erase(it);
			return c;
		}
	}
	return NULL;
}

Entity* Entity::RemoveChild(Entity* child){
	for(auto it = children.begin(); it != children.end(); it++) {
		Entity* c = (*it);
		if(c == child) {
			c->Remove();
			children.erase(it);
			return c;
		}
	}
	return NULL;
}

Entity* Entity::Clone(){
	Entity* clone = new Entity(this->name + "_copy");
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

Matrix Entity::GetWorldMatrix(){
	if(parent){
		return parent->GetTransform()->GetModelMatrix() * GetTransform()->GetModelMatrix();
	}else{
		return GetTransform()->GetModelMatrix();
	}
}

Vector3D Entity::GetDirection(){
	if(parent) {
		return parent->GetTransform()->GetModelMatrix() * GetTransform()->GetDirection();
	} else {
		return GetTransform()->GetDirection();
	}
}