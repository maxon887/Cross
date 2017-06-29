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
#include "Component.h"

namespace cross{

class RigidBody : public Component {
public:
	const float g = 10.f;

	RigidBody();
	RigidBody(float mass);

	virtual void Initialize();

	void PFXUpdate(float sec);

	void SetMass(float mass);
	float GetMass();

	void SetInverseMass(float iMass);
	float GetInverseMass();

	void SetVelocity(const Vector3D& vel);
	Vector3D GetVelocity();

	void SetAcceleration(const Vector3D& acc);
	Vector3D GetAcceleration();

	void SetDumping(float dumping);
	float GetDumping();

	void ApplyForce(const Vector3D& force);

private:
	Vector3D velocity;
	Vector3D acceleration;
	Vector3D force;
	float inverse_mass;
	float dumping;
};

}