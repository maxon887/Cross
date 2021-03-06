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

using namespace cross;

class Action {
	virtual void RestoreChanges() = 0;
};

class EntityChanged : public Action {
public:
	EntityChanged(Entity* previosState);

	void RestoreChanges() override;

private:
	Entity* original	= NULL;

	String name;
	Vector3D pos;
	Vector3D scale;
	Quaternion rot;
};

class EntityCreated : public Action {
public:
	EntityCreated(Entity* previosState);

	void RestoreChanges() override;

private:
	Entity* created_entity;
};

class EntityDeleted : public Action {
public:
	EntityDeleted(Entity* deletedEntity);

	void RestoreChanges() override;

private:
	Entity* deleted_entity;
};