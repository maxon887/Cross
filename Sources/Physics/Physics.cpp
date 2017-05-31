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
#include "Physics/Collider.h"
#include "System.h"

using namespace cross;

void Physics::Update(float sec){
	//forces update
	for(RigidBody* rigid : rigidbodyes){
		rigid->PFXUpdate(sec);
	}
	//collision finding
	for(CollisionProvider* provider : collision_providers){
		provider->Provide(collisions, colliders);
	}
	//collision resolving
	U32 iterations = collisions.size() * 2;
	for(U32 i = 0; i < iterations; ++i){
		float maxCV = 0;
		int maxIndex = 0;
		for(U32 j = 0; j < collisions.size(); ++j){
			float cv = CalcClosingVelocity(collisions[j]);
			if(cv > maxCV){
				maxCV = cv;
				maxIndex = j;
			}
		}
		if(maxCV <= 0) {
			break;
		}
		ResolveCollision(sec, collisions[maxIndex]);
		ResolveInterpenetration(collisions[maxIndex]);
	}
	collisions.clear();
}

void Physics::Clear(){
	rigidbodyes.clear();
	collisions.clear();
	colliders.clear();
	collision_providers.clear();
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

float Physics::CalcClosingVelocity(Collision& collision){
	Vector3D combinedVelocity = collision.first->GetVelocity() * (-1.f);

	if(collision.second) {
		combinedVelocity += collision.second->GetVelocity();
	}
	return Vector3D::Dot(collision.normal, combinedVelocity);
}

void Physics::ResolveCollision(float sec, Collision& collision){
	float closingVelocity = CalcClosingVelocity(collision);
	if(closingVelocity < 0){
		return;
	}

	float newClosingVelocity = -collision.restitution * closingVelocity;
	//resting contact
	Vector3D totalAcceleration = collision.first->GetAcceleration();

	if(collision.second){
		totalAcceleration -= collision.second->GetAcceleration();
	}
	
	float frameAcceleration = Vector3D::Dot(totalAcceleration, collision.normal) * sec;
	if(frameAcceleration < 0){
		newClosingVelocity -= collision.restitution * frameAcceleration;
		if(newClosingVelocity > 0){
			newClosingVelocity = 0;
		}
	}

	float deltaVelocity = closingVelocity - newClosingVelocity;

	float totalInverseMass = collision.first->GetInverseMass();
	if(collision.second){
		totalInverseMass += collision.second->GetInverseMass();
	}
	if(totalInverseMass <= 0){
		return;
	}

	float impulseVal = deltaVelocity / totalInverseMass;
	Vector3D impulse = collision.normal * impulseVal;
	collision.first->SetVelocity(collision.first->GetVelocity() + impulse * collision.first->GetInverseMass());
	if(collision.second){
		collision.second->SetVelocity(collision.second->GetVelocity() - impulse * collision.second->GetInverseMass());
	}
}

void Physics::ResolveInterpenetration(Collision& collision){
	if(collision.depth <= 0){
		return;
	}

	float totalInverseMass = collision.first->GetInverseMass();
	if(collision.second){
		totalInverseMass += collision.second->GetInverseMass();
	}

	if(totalInverseMass <= 0){
		return;
	}

	Vector3D movePerIMass = collision.normal * (collision.depth / totalInverseMass);

	Vector3D move1 = movePerIMass * collision.first->GetInverseMass();
	collision.first->SetPosition(collision.first->GetPosition() + move1);
	if(collision.second){
		Vector3D move2 = movePerIMass * collision.second->GetInverseMass() * (-1.f);
		collision.second->SetPosition(collision.second->GetPosition() + move2);
	}
	collision.depth = 0;
}