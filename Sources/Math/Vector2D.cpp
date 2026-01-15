#include "Vector2D.h"

#include <cmath>

using namespace cross;

float Vector2D::Dot(const Vector2D& left, const Vector2D& right){
	return left.x * right.x + left.y * right.y;
}

Vector2D::Vector2D():
	x(0.0f),
	y(0.0f)
{ }

Vector2D::Vector2D(float value):
	x(value),
	y(value)
{ }

Vector2D::Vector2D(float x, float y):
	x(x),
	y(y)
{ }

float Vector2D::Length() const {
	return (float)sqrt(x*x + y*y);
}

Vector2D Vector2D::GetNormalized() const {
	Vector2D result;
	float len = Length();
	result.x  = x / len;
	result.y  = y / len;
	return result;
}

Vector2D Vector2D::GetTruncated(float len) const {
	if(this->Length() > len){
		Vector2D result;
		result = this->GetNormalized();
		result *= len;
		return result;
	}
	return *this;
}

const float* Vector2D::GetData() const {
	return &x;
}

bool Vector2D::operator==(const Vector2D &v2) const {
	if(this->x == v2.x && this->y == v2.y) {
		return true;
	} else {
		return false;
	}
}

bool Vector2D::operator!=(const Vector2D &v2) const {
	return !((*this) == v2);
}

Vector2D Vector2D::operator+(const Vector2D &v2) const {
	return Vector2D(this->x + v2.x, this->y + v2.y);
}

Vector2D Vector2D::operator-(const Vector2D &v2) const {
	return Vector2D(this->x - v2.x, this->y - v2.y);
}

Vector2D Vector2D::operator*(float v) const {
	return Vector2D(this->x * v, this->y * v);
}

Vector2D Vector2D::operator/(float v) const {
	return Vector2D(this->x / v, this->y / v);
}


void Vector2D::operator+=(const Vector2D &v2) {
	this->x += v2.x;
	this->y += v2.y;
}

void Vector2D::operator-=(const Vector2D &v2) {
	this->x -= v2.x;
	this->y -= v2.y;
}

void Vector2D::operator*=(float v) {
	this->x *= v;
	this->y *= v;
}

void Vector2D::operator/=(float v) {
	this->x /= v;
	this->y /= v;
}