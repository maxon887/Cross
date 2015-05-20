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

class PointX;
class RectX;

bool PointInRect(PointX p, RectX rect);
bool PointInTriangle(PointX pt, PointX v1, PointX v2, PointX v3);
bool CircleOnCollision(PointX p1, float r1, PointX p2, float r2);
float Lerp(float v0, float v1, float t);
float Distance(PointX p1, PointX p2);
float Angle(PointX first, PointX second);