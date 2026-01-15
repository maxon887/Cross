#pragma once

namespace cross{

class Vector2D {
public:
	static float Dot(const Vector2D& left, const Vector2D& right);

	float x;
	float y;

	Vector2D();
	Vector2D(float value);
	Vector2D(float x, float y);

	float Length() const;
	Vector2D GetNormalized() const;
	Vector2D GetTruncated(float len) const;
	const float* GetData() const;

	bool operator == (const Vector2D &v2) const;
	bool operator != (const Vector2D &v2) const;

	Vector2D operator + (const Vector2D &v2) const;
	Vector2D operator - (const Vector2D &v2) const;
	Vector2D operator * (float value) const;
	Vector2D operator / (float value) const;

	void operator += (const Vector2D &v2);
	void operator -= (const Vector2D &v2);
	void operator *= (float value);
	void operator /= (float value);
};

}