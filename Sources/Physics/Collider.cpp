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
#include "Physics/Collider.h"
#include "RigidBody.h"

using namespace cross;

Collider::Collider() :
	Component(Type::COLLIDER)
{ }

void Collider::Resolve(Collider* other, Contact& contact){
	throw CrossException("Collider resolvation not implemented");
}

void Collider::ResolveInterpenetration(Collider* other, Contact& contact){
	if(contact.depth <= 0){
		return;
	}

	RigidBody* thisRB = (RigidBody*)GetComponent(Component::Type::RIGIDBODY);
	float totalInverseMass = thisRB->GetInverseMass();
	if(other){
		RigidBody* otherRB = (RigidBody*)other->GetComponent(Component::Type::RIGIDBODY);
		totalInverseMass += otherRB->GetInverseMass();
	}
	
	if(totalInverseMass <= 0){
		return;
	}

	Vector3D movePerIMass = contact.normal * (-contact.depth / totalInverseMass);

	this->SetPosition(GetPosition() + movePerIMass * thisRB->GetInverseMass());
	if(other){
		RigidBody* otherRB = (RigidBody*)other->GetComponent(Component::Type::RIGIDBODY);
		other->SetPosition(other->GetPosition() - movePerIMass * otherRB->GetInverseMass());
	}
}

float Collider::CalcSeparatingVelocity(Collider* other, Vector3D& contactNormal){
	RigidBody* thisRB = (RigidBody*)GetComponent(Component::Type::RIGIDBODY);
	Vector3D commonVel = thisRB->GetVelocity();
	if(other){
		RigidBody* otherRB = (RigidBody*)GetComponent(Component::Type::RIGIDBODY);
		commonVel -= otherRB->GetVelocity();
	}
	return Vector3D::Dot(commonVel, contactNormal);
}

