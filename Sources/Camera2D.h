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
#include "Cross.h"
#include "Camera.h"

namespace cross{

/*	This class allow you set 2D view distance by SetViewWidth. This allow you scale 
	2D sceene and keep abstract from physical screen ppi. In additional you you can
	scroll across your 2D sceene by SetPosition function */
class Camera2D : public Camera{
public:
	Camera2D();
	/* Set up camera view distance */
	void SetViewWidth(float width);
	/* Return width view distance */
	float GetViewWidth();
	/* Return height view distance */
	float GetViewHeight();
private:
	float view_width;
	float view_height;
};

}