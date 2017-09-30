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
#include "ScrollScreen.h"
#include "Menu.h"

using namespace cross;

class MainScreen : public ScrollScreen{
public:
	void Start() override;
	void Stop() override;
	void Update(float sec) override;

private:
	Font* font				= NULL;

	Menu* current_menu		= NULL;

	Menu* main_menu			= NULL;
	Menu* graphics2D_menu	= NULL;
	Menu* graphics3D_menu	= NULL;

	Menu* graphics3D_simple = NULL;
	Menu* graphics3D_light	= NULL;
	Menu* graphics3D_maps	= NULL;
	Menu* graphics3D_misc	= NULL;

	void SetMenu(Menu* newMenu);

	//connections
    U64 resize_del			= 0;
	void WindowResizedHandle(S32 width, S32 height);
};
