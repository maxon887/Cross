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
#include "Physics/PlaneCollider.h"

using namespace cross;

PlaneCollider::PlaneCollider(const Vector3D& normal, float distance) : 
	normal(normal),
	distance(distance)
{ }

Collision PlaneCollider::OnCollision(SphereCollider* other){
	float centerDistance = abs(Vector3D::Dot(normal, other->GetTransform()->GetPosition())) + distance;
	float distance = centerDistance - other->GetRadius();
	return Collision(distance > other->GetRadius(), distance);
}