#pragma once

namespace cross{

class Vector3D;

class Vector4D{
public:
	float x;
	float y;
	float z;
	float w;

	Vector4D();
	Vector4D(float x, float y, float z, float w);
	Vector4D(const Vector3D& vec, float w);

	const float* GetData() const;

	Vector4D operator + (const Vector4D &v2) const;
	Vector4D operator - (const Vector4D &v2) const;
	Vector4D operator * (float value) const;
	Vector4D operator / (float value) const;

	void operator += (const Vector4D &v2);
	void operator -= (const Vector4D &v2);
	void operator *= (float value);
	void operator /= (float value);
};

}