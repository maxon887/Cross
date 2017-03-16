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

using namespace cross;

RigidBody::RigidBody() :
	Component(Component::Type::RIGIDBODY),
	inverse_mass(1.f),
	velocity(0.f),
	use_gravity(true)
{ }

void RigidBody::Update(float sec){
	Transformable* trans = GetTransform();
	trans->SetPosition(trans->GetPosition() + velocity * sec);

	if(use_gravity){
		velocity += Vector3D(0.f, -10.f, 0.f) * sec;
	}
}

Vector3D RigidBody::GetVelocity() const{
	return velocity;
}

void RigidBody::AddVelocity(const Vector3D& vel){
	velocity += vel;
}

void RigidBody::SetVelocity(const Vector3D& vel){
	velocity = vel;
}

void RigidBody::SetMass(float mass){
	inverse_mass = 1.f / mass;
}

float RigidBody::GetInverseMass() const{
	return inverse_mass;
}

void RigidBody::UseGravity(bool yes){
	use_gravity = yes;
}