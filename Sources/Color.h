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

namespace cross {

class Color{
public:
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color White;
	static const Color Black;
	static const Color Yellow;
	static const Color Purple;
	static Color Random();

	float R, G, B, A;
	Color(float all);
	Color(float r, float g, float b);
	Color(int r, int g, int b);
	Color(float r, float g, float b, float a);
	Color(int r, int g, int b, int a);
	float* GetData();

	bool operator == (const Color &c) const;
	bool operator != (const Color &c) const;
};
    
}