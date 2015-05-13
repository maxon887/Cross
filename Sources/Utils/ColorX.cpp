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

ColorX ColorX::Red(RED);
ColorX ColorX::Green(GREEN);
ColorX ColorX::Blue(BLUE);
ColorX ColorX::White(WHITE);
ColorX ColorX::Black(BLACK);


ColorX::ColorX(float r, float g, float b){
	this->R = r;
	this->G = g;
	this->B = b;
}

ColorX::ColorX(COLORX c){
	switch (c)
	{
	case RED:
		R = 1.f;
		G = 0.f;
		B = 0.f;
		break;
	case GREEN:
		R = 0.f;
		G = 1.f;
		B = 0.f;
		break;
	case BLUE:
		R = 0.f;
		G = 0.f;
		B = 1.f;
		break;
	case WHITE:
		R = 1.f;
		G = 1.f;
		B = 1.f;
		break;
	case BLACK:
		R = 0.f;
		G = 0.f;
		B = 0.f;
		break;
	default:
		R = 0.f;
		G = 0.f;
		B = 0.f;
		break;
	}
}