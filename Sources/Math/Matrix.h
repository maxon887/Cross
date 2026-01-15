#pragma once

namespace cross{

class Vector2D;
class Vector3D;
class Vector4D;

class Matrix{
public:
	static const Matrix Zero;
	static const Matrix Identity;

	static Matrix CreateTranslation(const Vector3D &vec);
	static Matrix CreateScale(const Vector3D &scale);
	static Matrix CreateOrthogonalProjection(float left, float right, float bottom, float top, float near, float far);
	static Matrix CreatePerspectiveProjection(float fov, float aspect, float near, float far);

	float m[4][4];

	void SetTranslation(const Vector2D &trans);
	void SetTranslation(const Vector3D &trans);
	Matrix GetTranslation() const;
	Matrix GetScale() const;
	Matrix GetRotation() const;
	void SetScale(float scale);
	void SetScale(const Vector2D &scale);
	void SetScale(const Vector3D &scale);
	void SetRotationX(float angle);
	void SetRotationY(float angle);
	void SetRotationZ(float angle);
	Matrix GetTransposed() const;
	Matrix GetInversed() const;
	const float* GetData() const;

	Matrix		operator + (float s) const;
	Matrix		operator - (float s) const;
	Matrix		operator * (float s) const;
	Matrix		operator / (float s) const;
	Vector4D	operator * (const Vector4D& vec) const;
	Vector3D	operator * (const Vector3D& vec) const;
	Matrix		operator * (const Matrix& m2) const;
	bool		operator == (const Matrix& m2) const;
	bool		operator != (const Matrix& m2) const;

	void operator += (float s);
	void operator -= (float s);
	void operator *= (float s);
	void operator /= (float s);

private:
	static Matrix CreateZero();
	static Matrix CreateIdentity();

	Matrix() = default;
};

}