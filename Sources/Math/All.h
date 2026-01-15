#pragma once
#include "Rect.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"
#include "Quaternion.h"

#define PI (3.141592653589793f)

namespace cross {

int Random(int max);
int Random(int min, int max);
float Random(float max);
float Random(float min, float max);
/* Return closest whole number. Mathematical round rules */
float Round(float val);
float Lerp(float v1, float v2, float t);
Vector3D Lerp(const Vector3D& v1, const Vector3D& v2, float t);
Quaternion Lerp(const Quaternion& v1, const Quaternion& v2, float t);

bool PointInRect(const Vector2D& p, const Rect& rect);
bool PointInTriangle(const Vector2D& pt, const Vector2D& v1, const Vector2D& v2, const Vector2D& v3);
bool PointInCircle(const Vector2D& p, const Vector2D& center, float radius);
float Distance(const Vector2D &v1, const Vector2D &v2);
float DistanceSq(const Vector2D &v1, const Vector2D &v2);
float Distance(const Vector3D &v1, const Vector3D &v2);
float DistanceSq(const Vector3D &v1, const Vector3D &v2);

}
