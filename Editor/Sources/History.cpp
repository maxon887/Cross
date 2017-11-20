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
#include "History.h"
#include "Entity.h"
#include "System.h"
#include "Transform.h"

EntityChanged::EntityChanged(Entity* entity) 
{
	original = entity;

	name = entity->GetName();
	pos = entity->GetTransform()->GetPosition();
	scale = entity->GetTransform()->GetScale();
	rot = entity->GetTransform()->GetRotate();
}

void EntityChanged::RestoreChanges() {
	CROSS_FAIL(original, "Original entity has been deleted");
	original->SetName(name);
	original->GetTransform()->SetPosition(pos);
	original->GetTransform()->SetScale(scale);
	original->GetTransform()->SetRotate(rot);
}

EntityCreated::EntityCreated(Entity* entity) {
	created_entity = entity;
}

void EntityCreated::RestoreChanges() {
	Entity* parent = created_entity->GetParent();
	parent->RemoveChild(created_entity);
	delete created_entity;
}

EntityDeleted::EntityDeleted(Entity* entity) {
	deleted_entity = entity;
}

void EntityDeleted::RestoreChanges() {
	deleted_entity->GetParent()->AddChild(deleted_entity);
}