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
#include "Physics/Physics.h"
#include "Physics/RigidBody.h"

using namespace cross;

void Physics::Update(float sec){
	//collision finding
	for(CollisionProvider* provider : collision_providers){
		provider->Provide(collisions, colliders);
	}
	//collision resolving
	while(!collisions.empty()){
		Collision col = collisions.back();
		collisions.pop_back();
		while(!col.contacts.empty()){
			Collision::Contact contact = col.contacts.back();
			col.contacts.pop_back();
			ResolveCollision(col.first, col.second, contact);
		}
	}
}

void Physics::RegisterCollider(Collider* collider){
	colliders.push_back(collider);
}

void Physics::RegisterCollisionProvider(CollisionProvider* provider){
	collision_providers.push_back(provider);
}

void Physics::ResolveCollision(Collider* first, Collider* second, Collision::Contact& contact){
	RigidBody* firstRB = NULL;
	RigidBody* secondRB = NULL;
	if(!first->HasComponent(Component::RIGIDBODY)){
		return;
	} else {
		firstRB = (RigidBody*)first->GetComponent(Component::RIGIDBODY);
		if(second){
			if(!second->HasComponent(Component::RIGIDBODY)){
				return;
			}else{
				secondRB = (RigidBody*)second->GetComponent(Component::RIGIDBODY);
			}
		}
	}

	Vector3D relativeVelocity = firstRB->GetVelocity() * (-1.f);
	if(secondRB){
		relativeVelocity += secondRB->GetVelocity();
	}
	float closingVelocity = Vector3D::Dot(contact.normal, relativeVelocity);

	if(closingVelocity < 0){
		return;
	}

	float deltaVelocity = 2.f * closingVelocity;

	float totalInverseMass = firstRB->GetInverseMass();
	if(secondRB){
		totalInverseMass += secondRB->GetInverseMass();
	}
	if(totalInverseMass <= 0){
		return;
	}

	float impulseVal = deltaVelocity / totalInverseMass;
	Vector3D impulse = contact.normal * impulseVal;
	firstRB->SetVelocity(firstRB->GetVelocity() + impulse * firstRB->GetInverseMass());
	if(secondRB){
		secondRB->SetVelocity(secondRB->GetVelocity() - impulse * secondRB->GetInverseMass());
	}
}
