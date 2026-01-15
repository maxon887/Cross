#pragma once
#include "Cross.h"
#include "Math/All.h"

namespace cross {

class PrimitiveDrawer {
public:
	//2D drawing
	static void DrawPoint(const Vector2D& pos, Color& color);
	static void DrawLine(const Vector2D& p1, const Vector2D& p2, Color& color);
	static void DrawRect(const Rect& rect, Color& color);
	static void DrawRect(const Rect& rect, Color& color, bool filled);
	static void DrawCircle(const Vector2D& center, float radius, Color& color);
	static void DrawCircle(const Vector2D& center, float radius, Color& color, bool filled);
	static void DrawCircle(const Vector2D& center, float radius, Color& color, bool filled, U32 accuracy);
	//3d drawing
	static void DrawLine(const Vector3D& p1, const Vector3D& p2, Color& c);
};
	
}
