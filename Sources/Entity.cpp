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
#include "Component.h"

using namespace cross;

Entity::Entity(){
	memset(components, 0, sizeof(components));
}

Entity::~Entity(){
	for(Component* component : components) {
		if(component) {
			delete component;
		}
	}
}

void Entity::Update(float sec){
	for(Component* component : components){
		if(component){
			component->Update(sec);
		}
	}
}

void Entity::AddComponent(Component* component){
	if(components[component->GetType()] == NULL){
		components[component->GetType()] = component;
		component->Initialize(this);
	}else{
		throw CrossException("Entity already have same component");
	}
}

Component* Entity::GetComponent(Component::Type type){
	return components[type];
}