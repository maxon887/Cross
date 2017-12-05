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
#include "Cross.h"
#include "Component.h"

namespace cross{

/* With this class you can add spatial properties to any game object.
   Then rotate, scale and move object in world space. */
class Transform : public Component {
public:
	Transform() = default;
	Transform(const Vector3D& position);

	Component* Clone() const override;
	bool Load(tinyxml2::XMLElement* xml, Scene* laodingScene) override;
	bool Save(tinyxml2::XMLElement* xml, tinyxml2::XMLDocument* doc) override;

	void SetPosition(const Vector2D& pos);
	void SetPosition(const Vector3D& pos);
	void SetPosition(const Matrix& pos);
	Vector3D GetPosition() const;

	void SetScale(float factor);
	void SetScale(const Vector2D& scale);
	void SetScale(const Vector3D& scale);
	void SetScale(const Matrix& scale);
	Vector3D GetScale() const;

	void SetRotateX(float angle);
	void SetRotateY(float angle);
	void SetRotateZ(float angle);
	void SetRotate(const Vector3D& axis, float angle);
	void SetRotate(const Quaternion& quat);
	void SetRotate(const Matrix& rotation);
	Quaternion GetRotate() const;

	void LookAt(const Vector3D& object);

	void SetDirection(const Vector3D& direction);
	Vector3D GetDirection() const;
	Vector3D GetForward() const;
	Vector3D GetRight() const;
	Vector3D GetUp() const;

	Matrix GetRotation() const;

	Matrix& GetModelMatrix();

	void SetModelMatrix(const Matrix& model);

protected:
	Matrix model		= Matrix::Identity;
	Matrix translate	= Matrix::Identity;
	Matrix rotate		= Matrix::Identity;
	Matrix scale		= Matrix::Identity;
	bool recalc_model	= true;
};

}