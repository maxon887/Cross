#pragma once

namespace cross{

class Vector2D;
class Vector4D;

class Vector3D {
public:
	static const Vector3D Zero;
	static const Vector3D Up;
	static const Vector3D Forward;
	static const Vector3D Right;

	static Vector3D Cross(const Vector3D& left, const Vector3D& right);
	static float Dot(const Vector3D& left, const Vector3D& right);

	float x;
	float y;
	float z;

	Vector3D();
	Vector3D(float value);
	Vector3D(float x, float y, float z);
	Vector3D(const Vector2D& vec);
	Vector3D(const Vector2D& vec, float z);
	Vector3D(const Vector4D& vec);

	float Length() const;
	void Normalize();
	Vector3D GetNormalized() const;
	bool IsNormalized() const;
	Vector3D GetTruncated(float len) const;
	float* GetData();
	void SetData(const float* data);

	bool operator == (const Vector3D& v2) const;
	bool operator != (const Vector3D& v2) const;

	Vector3D operator + (const Vector3D& v2) const;
	Vector3D operator - (const Vector3D& v2) const;
	Vector3D operator * (float value) const;
	Vector3D operator / (float value) const;

	void operator += (const Vector3D &v2);
	void operator -= (const Vector3D &v2);
	void operator *= (float value);
	void operator /= (float value);
};

}