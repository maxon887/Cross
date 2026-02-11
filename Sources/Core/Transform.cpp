#include "Transform.h"
#include "Entity.h"

using namespace cross;

Transform::Transform() : Component("Transform")
{ }

Transform::Transform(const Vector3D& position) : Component("Transform") {
	SetPosition(position);
}

bool Transform::Activate() {
	position.ValueChanged.Connect(this, &Transform::ValueChanged);
	rotation.ValueChanged.Connect(this, &Transform::ValueChanged);
	scale.ValueChanged.Connect(this, &Transform::ValueChanged);
	return true;
}

void Transform::Deactivate() {
	position.ValueChanged.Disconnect(this, &Transform::ValueChanged);
	rotation.ValueChanged.Disconnect(this, &Transform::ValueChanged);
	scale.ValueChanged.Disconnect(this, &Transform::ValueChanged);
}

Component* Transform::Clone() const {
	Transform* result = CREATE Transform();
	result->position = this->position;
	result->scale = this->scale;
	result->rotation = this->rotation;
	result->recalc_model = true;
	return result;
}

Vector3D Transform::GetPosition() const {
	return position;
}

Vector3D Transform::GetWorldPosition() {
	Matrix world = GetWorldMatrix();
	Vector3D pos;
	pos.x = world.m[0][3];
	pos.y = world.m[1][3];
	pos.z = world.m[2][3];
	return pos;
}

void Transform::SetPosition(const Vector3D& pos) {
	position = pos;
}

void Transform::SetPosition(const Matrix& pos) {
	Vector3D newPosition(pos.m[0][3], pos.m[1][3], pos.m[2][3]);
	position = newPosition;
}

Vector3D Transform::GetScale() const {
	return scale;
}

void Transform::SetScale(float factor) {
	scale = Vector3D(factor);
}

void Transform::SetScale(const Vector2D& scaleVec) {
	scale = scaleVec;
}

void Transform::SetScale(const Vector3D& scaleVec) {
	scale = scaleVec;
}

void Transform::SetScale(const Matrix& scaleMat) {
	Vector3D newScale(scaleMat.m[0][0], scaleMat.m[1][1], scaleMat.m[2][2]);
	scale = newScale;
}

Quaternion Transform::GetRotate() const {
	return rotation;
}

void Transform::SetRotate(const Vector3D& axis, float angle) {
	Quaternion quat(axis, angle);
	SetRotate(quat);
}

void Transform::SetRotate(const Quaternion& quat) {
	rotation = quat.GetMatrix();
}

void Transform::SetRotate(const Matrix& rot) {
	rotation = rot;
}

void Transform::LookAt(const Vector3D& object){
	Vector3D forward = object - GetPosition();
	forward.Normalize();
	Vector3D right = Vector3D::Cross(Vector3D::Up, forward).GetNormalized();
	Vector3D up =  Vector3D::Cross(forward, right).GetNormalized();

	Matrix rot = Matrix::Identity;

	rot.m[0][0] = right.x;
	rot.m[1][0] = right.y;
	rot.m[2][0] = right.z;

	rot.m[0][1] = up.x;
	rot.m[1][1] = up.y;
	rot.m[2][1] = up.z;

	rot.m[0][2] = forward.x;
	rot.m[1][2] = forward.y;
	rot.m[2][2] = forward.z;

	rotation = rot;
	
	recalc_model = true;
}

Vector3D Transform::GetDirection() const {
	return GetForward();
}

Vector3D Transform::GetWorldDirection() {
	Matrix world = GetWorldMatrix();
	return world.GetRotation() * Vector3D::Forward;
}

Vector3D Transform::GetForward() const {
	return rotation.Get() * Vector3D::Forward;
}

Vector3D Transform::GetRight() const {
	return rotation.Get() * Vector3D::Right;
}

Vector3D Transform::GetUp() const {
	return rotation.Get() * Vector3D::Up;
}

void Transform::SetDirection(const Vector3D& direction) {
	Vector3D lookAt = this->GetPosition() + direction;
	LookAt(lookAt);
}

Matrix& Transform::GetModelMatrix() {
	if(recalc_model) {
		Matrix translate = Matrix::Identity;
		translate.SetTranslation(position);
		Matrix scaleMat = Matrix::Identity;
		scaleMat.SetScale(scale);
		model = translate * rotation.Get().GetMatrix() * scaleMat;
		recalc_model = false;
	}
	return model;
}

Matrix Transform::GetWorldMatrix() {
	Entity* parent = GetEntity()->GetParent();
	if(parent) {
		return parent->GetComponent<Transform>()->GetWorldMatrix() * GetTransform()->GetModelMatrix();
	} else {
		return GetTransform()->GetModelMatrix();
	}
}

void Transform::ValueChanged() {
	recalc_model = true;
}