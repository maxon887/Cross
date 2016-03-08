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
#include "Screen.h"
#include "Utils/Button.h"
#include "Menu.h"

using namespace cross;

class MainScreen : public Screen{
public:
	void Start();
	void Stop();
	void Update(float sec);
private:
	Menu* current_menu;
	Menu* main_menu;
	Menu* graphics2D_menu;
	Menu* graphics3D_menu;
	Sprite* button_sprite;
	Screen* next_screen;
	//connections
	void OnGraphics2DClick();
	void OnGraphics3DClick();
	void OnPrimitivesClick();
	void OnSpritesClick();
	void OnAnimationClick();
	void OnTestNaPidoraClick();
	void On2DCameraClick();
	void OnTextClick();
	void OnTriangleClick();
	void OnAudioClick();
};
