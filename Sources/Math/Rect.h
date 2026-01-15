#pragma once

namespace cross {

class Vector2D;

class Rect{
public:
	float x;
	float y;
	float width;
	float height;

	Rect();
	Rect(const Vector2D& pos, float width, float height);
	Rect(float x, float y, float width, float height);
};

}