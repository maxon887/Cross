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
#include "Physics/Bridge.h"
#include "Physics/RigidBody.h"

void ParticleCollider::Resolve(Collider* other, Contact& contact){
	RigidBody* thisRB = (RigidBody*)GetComponent(Component::Type::RIGIDBODY);

	float separatingVelocity = CalcSeparatingVelocity(other, contact.normal);
	if(separatingVelocity > 0){
		return;
	}
	float resSepVel = -separatingVelocity;
	float totalInverseMass = thisRB->GetInverseMass();
	if(other){
		RigidBody* otherRB = (RigidBody*)GetComponent(Component::Type::RIGIDBODY);
		totalInverseMass += otherRB->GetInverseMass();
	}
	if(totalInverseMass <= 0){
		return;
	}
	//ad hoc
	float impulse = -2 * separatingVelocity / totalInverseMass;
	Vector3D impulseVec = contact.normal * impulse;

	thisRB->SetVelocity(thisRB->GetVelocity() + impulseVec * thisRB->GetInverseMass());
	if(other){
		RigidBody* otherRB = (RigidBody*)GetComponent(Component::Type::RIGIDBODY);
		otherRB->SetVelocity(otherRB->GetVelocity() - impulseVec * thisRB->GetInverseMass());
	}
}

void Bridge::Start(){
	CameraControlsScene::Start();
	SetBackground(Color(0.3f));
}

void Bridge::Stop(){
	CameraControlsScene::Stop();
}

void Bridge::Update(float sec){
	CameraControlsScene::Update(sec);
}