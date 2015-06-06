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
	
#include "ColorX.h"

const ColorX ColorX::Red(1.f, 0.f, 0.f);
const ColorX ColorX::Green(0.f, 1.f, 0.f);
const ColorX ColorX::Blue(0.f, 0.f, 1.f);
const ColorX ColorX::White(1.f, 1.f, 1.f);
const ColorX ColorX::Black(0.f, 0.f, 0.f);


ColorX::ColorX(float r, float g, float b){
	this->R = r;
	this->G = g;
	this->B = b;
}