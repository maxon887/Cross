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
#include "Transform.h"
#include "Physics/Physics.h"

using namespace cross;

RigidBody::RigidBody() :
	Component("RigidBody"),
	velocity(Vector3D::Zero),
	acceleration(Vector3D::Zero),
	force(Vector3D::Zero),
	inverse_mass(1.f),
	dumping(0.f)
{
	acceleration.y = -g;
}

RigidBody::RigidBody(float mass) :
	Component("RigidBody"),
	acceleration(Vector3D::Zero),
	force(Vector3D::Zero),
	velocity(Vector3D::Zero),
	dumping(0.f)
{ 
	SetMass(mass);
	acceleration.y = -g;
}

void RigidBody::Initialize(Scene*){
	physics->RegisterRigidBody(this);
}

void RigidBody::PFXUpdate(float sec){
	Vector3D resAccel = acceleration;
	resAccel += force * inverse_mass;

	force = Vector3D::Zero;
	velocity += resAccel * sec;
	velocity *= powf(1.f - dumping, sec);

	GetEntity()->GetTransform()->SetPosition(GetEntity()->GetTransform()->GetPosition() + velocity * sec);
}

void RigidBody::SetMass(float mass){
	inverse_mass = 1.f / mass;
}

float RigidBody::GetMass(){
	return 1.f / inverse_mass;
}

void RigidBody::SetInverseMass(float iMass){
	inverse_mass = iMass;
}

float RigidBody::GetInverseMass(){
	return inverse_mass;
}

void RigidBody::SetVelocity(const Vector3D& vel){
	velocity = vel;
}

Vector3D RigidBody::GetVelocity(){
	return velocity;
}

void RigidBody::SetAcceleration(const Vector3D& acc){
	acceleration = acc;
}

Vector3D RigidBody::GetAcceleration(){
	return acceleration;
}

void RigidBody::SetDumping(float d){
	dumping = d;
}

float RigidBody::GetDumping(){
	return dumping;
}

void RigidBody::ApplyForce(const Vector3D& f){
	force += f;
}