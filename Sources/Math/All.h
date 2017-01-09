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
#include "Rect.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"
#include "Quaternion.h"

#include <math.h>
#include <sstream>

#define PI (3.141592653589793f)

#if defined(ANDROID)
template <typename T>
inline std::string to_stringAndroid(T value){
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}
#define  to_string to_stringAndroid
#endif

namespace cross {

int Random(int max);
int Random(int min, int max);
float Random(float max);
float Random(float min, float max);
bool PointInRect(const Vector2D& p, const Rect& rect);
bool PointInTriangle(const Vector2D& pt, const Vector2D& v1, const Vector2D& v2, const Vector2D& v3);
bool PointInCircle(const Vector2D& p, const Vector2D& center, float radius);
bool CircleOnCollision(const Vector2D& p1, float r1, const Vector2D& p2, float r2);
float Distance(const Vector2D &v1, const Vector2D &v2);
float DistanceSq(const Vector2D &v1, const Vector2D &v2);
float Distance(const Vector3D &v1, const Vector3D &v2);
float DistanceSq(const Vector3D &v1, const Vector3D &v2);

}
