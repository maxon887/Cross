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
#include "RigidBody.h"
#include "Entity.h"
#include "Physics/Physics.h"

using namespace cross;

RigidBody::RigidBody() : 
	Component(Component::Type::RIGIDBODY),
	velocity(Vector3D::Zero),
	inverse_mass(1.f),
	use_gravity(true)
{ }

RigidBody::RigidBody(float mass) :
	Component(Component::Type::RIGIDBODY),
	velocity(Vector3D::Zero),
	use_gravity(true)
{ 
	SetMass(mass);
}

void RigidBody::Update(float sec){
	if(use_gravity){
		velocity.y -= physics->g * sec;
	}

	velocity += force * inverse_mass;
	force = Vector3D::Zero;

	GetEntity()->SetPosition(GetEntity()->GetPosition() + velocity * sec);
}

void RigidBody::SetMass(float mass){
	inverse_mass = 1.f / mass;
}

void RigidBody::SetInverseMass(float iMass){
	inverse_mass = iMass;
}

void RigidBody::SetVelocity(const Vector3D& vel){
	velocity = vel;
}

void RigidBody::ApplyForce(const Vector3D& f){
	force += f;
}