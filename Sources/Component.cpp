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
#include "Component.h"
#include "Entity.h"

using namespace cross;

Component::Component(Type type) :
	type(type),
	entity(NULL)
{ }

void Component::Update(float sec){
	throw CrossException("Unimplemented component");
}

void Component::Initialize(Entity* entity){
	this->entity = entity;
}

bool Component::Initialized() const{
	return entity != NULL;
}

Component::Type Component::GetType() const{
	return type;
}

Entity* Component::GetEntity(){
	if(entity){
		return entity;
	}else{
		throw CrossException("Component not initialized");
	}
}

Component* Component::GetComponent(Component::Type type){
	return GetEntity()->GetComponent(type);
}