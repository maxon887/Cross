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
#include "Game.h"
#include "Utils/Button.h"

using namespace cross;

class Demo;
extern Demo* demo;

class Demo : public Game{
public:
	Demo();

	void Start() override;
	void Stop() override;
	void Update(float sec) override;
	Screen* GetStartScreen() override;

	Sprite* GetCommonSprite(string name);

private:
	Camera2D* camera;
    Button* back_btn;
	Texture* common_texture;
	Dictionary<string, Sprite*> common_sprites;

	void OnBackClick();
	void OnKeyPressed(Key key);
};
