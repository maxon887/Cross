#include "Vector4D.h"
#include "Vector3D.h"

using namespace cross;

Vector4D::Vector4D():
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(0.0f)
{ }

Vector4D::Vector4D(float x, float y, float z, float w):
	x(x),
	y(y),
	z(z),
	w(w)
{ }

Vector4D::Vector4D(const Vector3D& vec, float w):
	x(vec.x),
	y(vec.y),
	z(vec.z),
	w(w)
{ }

const float* Vector4D::GetData() const {
	return &x;
}

Vector4D Vector4D::operator+(const Vector4D &v2) const {
	return Vector4D(this->x + v2.x, this->y + v2.y, this->z + v2.z, this->w + v2.w);
}

Vector4D Vector4D::operator-(const Vector4D &v2) const {
	return Vector4D(this->x - v2.x, this->y - v2.y, this->z - v2.z, this->w - v2.w);
}

Vector4D Vector4D::operator*(float v) const {
	return Vector4D(this->x * v, this->y * v, this->z * v, this->w * v);
}

Vector4D Vector4D::operator/(float v) const {
	return Vector4D(this->x / v, this->y / v, this->z / v, this->w / v);
}

void Vector4D::operator += (const Vector4D &v2) {
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
	this->w += v2.w;
}

void Vector4D::operator -= (const Vector4D &v2) {
	this->x -= v2.x;
	this->y -= v2.y;
	this->z -= v2.z;
	this->w -= v2.w;
}

void Vector4D::operator *= (float v) {
	this->x *= v;
	this->y *= v;
	this->z *= v;
}

void Vector4D::operator /= (float v) {
	this->x /= v;
	this->y /= v;
	this->z /= v;
	this->w /= v;
}