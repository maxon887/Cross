/*	Copyright © 2018 Maksim Lukyanov

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
#include "Cross.h"
#include "Component.h"

namespace cross{

/*	Adds spatial properties to an Entity. Like position, rotation and scale in virtual world.
	Children objects will be have related to parent properties and will be transformed with parents */
class Transform : public Component {
public:
	Transform();
	Transform(const Vector3D& position);

	/* Full by per element object copy operation */
	Component* Clone() const override;

	/* Returns position of this object in a relative coordinates */
	Vector3D GetPosition() const;
	/* Returns position of this object in a world coordinates */
	Vector3D GetWorldPosition();
	/* Sets position of this object by 3D vector */
	void SetPosition(const Vector3D& pos);
	/* Sets position of this object by translation Matrix */
	void SetPosition(const Matrix& pos);

	/* Returns object scale as Vector3D where x, y, z components represent object scaling with x, y, z axis of object coordinate */
	Vector3D GetScale() const;
	/* Sets object scale by float, all object axis will be scaled with same value */
	void SetScale(float factor);
	/* Sets object scale by 2D vector */
	void SetScale(const Vector2D& scale);
	/* Sets object scale by 3D vector */
	void SetScale(const Vector3D& scale);
	/* Sets object scale by scale Matrix */
	void SetScale(const Matrix& scale);

	/* Returns object rotation as Quaternion. To get actuial axis and angle use Quaternion::GetAxis() and Quaternion::GetAngle() */
	Quaternion GetRotate() const;
	/* Sets object rotation as axis and angle (in degrees) */
	void SetRotate(const Vector3D& axis, float angle);
	/* Sets object rotation by Quaternion, default representation of rotation */
	void SetRotate(const Quaternion& quat);
	/* Sets object rotation by rotation Matrix */
	void SetRotate(const Matrix& rotation);

	/* Rotates object in way to object's direction vector look at provided object */
	void LookAt(const Vector3D& object);

	/* Returns object direction vector. Object forwarding direction */
	Vector3D GetDirection() const;
	/* Returns object forward vector. Same as direction vector */
	Vector3D GetForward() const;
	/* Returns local object right axis */
	Vector3D GetRight() const;
	/* Returns local object up axis */
	Vector3D GetUp() const;
	/* Sets object forward direction */
	void SetDirection(const Vector3D& direction);

	/* Returns object models Matrix. Usually need for shader calculation */
	Matrix& GetModelMatrix();
	/* Returns world transform Matrix. Not fast and save function (all parents matrices must be multiplied and must exists) */
	Matrix GetWorldMatrix();

protected:
	Property<Vector3D> position		= Property<Vector3D>(this, "Position", Vector3D::Zero);
	Property<Vector3D> scale 		= Property<Vector3D>(this, "Scale", Vector3D(1.f));
	Property<Quaternion> rotation	= Property<Quaternion>(this, "Rotation", Quaternion::Identity);

	Matrix model		= Matrix::Identity;
	bool recalc_model	= true;
};

}