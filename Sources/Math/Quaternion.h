#pragma once

namespace cross{

class Vector3D;
class Matrix;

class Quaternion {
public:
	static const Quaternion Identity;
	/* Returns Cos of half angle between quaternions multiplied by their lengths.
	 * In case of normalized quats you will get cos(Alpha/2) */
	static float DotProduct(const Quaternion& left, const Quaternion& right);
	/* Creates identity quaternion */
	Quaternion();
	/* Constructor which takes rotation axis and angle in degrees */
	Quaternion(const Vector3D& axis, float angle);
	/* Constructor which create quaternion by rotation matrix */
	Quaternion(const Matrix& rotation);

	void Scale(float scale);
	float Norm() const;  
	float Length() const;
	/* Creates quaternion with opposite rotation */
	Quaternion GetConjugated() const;
	Quaternion GetInversed() const;
	Quaternion GetNormalized() const;
	Matrix GetMatrix() const;
	Vector3D GetAxis() const;
	float GetAngle() const;

	Quaternion operator + (const Quaternion& q) const;
	Quaternion operator - (const Quaternion& q) const;
	Quaternion operator * (const Quaternion& q) const;
	Vector3D   operator * (const Vector3D& vec) const;
	Quaternion operator * (float value) const;
	Quaternion operator / (float value) const;

	float x;
	float y;
	float z;
	float w;
};

}