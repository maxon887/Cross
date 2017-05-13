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
#include "System.h"

using namespace cross;

void Physics::Update(float sec){
	//forces update
	for(RigidBody* rigid : rigidbodyes){
		rigid->PhysicUpdate(sec);
	}
	//collision finding
	for(CollisionProvider* provider : collision_providers){
		provider->Provide(collisions, colliders);
	}
	//collision resolving
	while(!collisions.empty()){
		Collision col = collisions.back();
		collisions.pop_back();
		RigidBody* firstRB = NULL;
		RigidBody* secondRB = NULL;
		if(!col.first->HasComponent(Component::RIGIDBODY)) {
			continue;
		} else {
			firstRB = (RigidBody*)col.first->GetComponent(Component::RIGIDBODY);
			if(col.second) {
				if(!col.second->HasComponent(Component::RIGIDBODY)) {
					continue;
				} else {
					secondRB = (RigidBody*)col.second->GetComponent(Component::RIGIDBODY);
				}
			}
		}
		// WARNIG May cause a problem
		int interations = col.contacts.size();
		for(int i = 0; i < interations; ++i){
			float maxCV = 0;
			int index = 0;
			for(int i = 0; i < col.contacts.size(); ++i){
				float CV = CalcClosingVelocity(firstRB, secondRB, col.contacts[i]);
				if(CV > maxCV){
					maxCV = CV;
					index = i;
				}
			}
			ResolveCollision(sec, firstRB, secondRB, col.contacts[index]);
			ResolveInterpenetration(firstRB, secondRB, col.contacts[index]);
		}
		/* Old Collision resolver code
		while(!col.contacts.empty()){
			Collision::Contact contact = col.contacts.back();
			col.contacts.pop_back();
			ResolveCollision(sec, firstRB, secondRB, contact);
			ResolveInterpenetration(firstRB, secondRB, contact);
		}*/
	}
}

void Physics::RegisterRigidBody(RigidBody* rigid){
	rigidbodyes.push_back(rigid);
}

void Physics::RegisterCollider(Collider* collider){
	colliders.push_back(collider);
}

void Physics::RegisterCollisionProvider(CollisionProvider* provider){
	collision_providers.push_back(provider);
}

float Physics::CalcClosingVelocity(RigidBody* first, RigidBody* second, Collision::Contact& contact){
	Vector3D combinedVelocity = first->GetVelocity() * (-1.f);
	if(second) {
		combinedVelocity += second->GetVelocity();
	}
	return Vector3D::Dot(contact.normal, combinedVelocity);
}

void Physics::ResolveCollision(float sec, RigidBody* first, RigidBody* second, Collision::Contact& contact){
	float closingVelocity = CalcClosingVelocity(first, second, contact);
	if(closingVelocity < 0){
		return;
	}

	float newClosingVelocity = -contact.restitution * closingVelocity;
	//resting contact
	Vector3D totalAcceleration = first->GetAcceleration();
	if(second){
		totalAcceleration -= second->GetAcceleration();
	}
	
	float frameAcceleration = Vector3D::Dot(totalAcceleration, contact.normal) * sec;
	if(frameAcceleration < 0){
		newClosingVelocity -= contact.restitution * frameAcceleration;
		if(newClosingVelocity > 0){
			newClosingVelocity = 0;
		}
	}

	float deltaVelocity = closingVelocity - newClosingVelocity;

	float totalInverseMass = first->GetInverseMass();
	if(second){
		totalInverseMass += second->GetInverseMass();
	}
	if(totalInverseMass <= 0){
		return;
	}

	float impulseVal = deltaVelocity / totalInverseMass;
	Vector3D impulse = contact.normal * impulseVal;
	first->SetVelocity(first->GetVelocity() + impulse * first->GetInverseMass());
	if(second){
		second->SetVelocity(second->GetVelocity() - impulse * second->GetInverseMass());
	}
}

void Physics::ResolveInterpenetration(RigidBody* first, RigidBody* second, Collision::Contact& contact){
	if(contact.depth <= 0){
		return;
	}

	float totalInverseMass = first->GetInverseMass();
	if(second){
		totalInverseMass += second->GetInverseMass();
	}

	if(totalInverseMass <= 0){
		return;
	}

	Vector3D movePerIMass = contact.normal * (contact.depth / totalInverseMass);

	first->SetPosition(first->GetPosition() + movePerIMass * first->GetInverseMass());
	if(second){
		second->SetPosition(second->GetPosition() + movePerIMass * second->GetInverseMass());
	}
}